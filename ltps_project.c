#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ROWS         10     /* seat rows per bus */
#define COLS         4      /* A B | C D : A and D are windows, B and C are aisle */
#define MAX_BUSES    4
#define MAX_BOOKINGS 200
#define NAME_LEN     50

struct Bus {
    int  id;
    char route[64];
    char depart[8];
    int  fare;
    int  seats[ROWS][COLS];   /* 0 = free, otherwise the ticket ID that holds it */
};

struct Booking {
    int  ticket;
    char name[NAME_LEN];
    int  bus;                 /* index into buses[] */
    int  row, col;
    int  fare;
    int  active;              /* 0 once cancelled */
};

typedef struct {
    int row, col, score;      /* lower score = better match */
} Candidate;

static struct Bus     buses[MAX_BUSES];
static int            bus_count = 0;
static struct Booking bookings[MAX_BOOKINGS];
static int            booking_count = 0;
static int            next_ticket = 1001;

/* ---------- input helpers ---------- */

/* Reads one line without the newline. Returns 0 on end of input. */
static int read_line(const char *prompt, char *buf, size_t size) {
    printf("%s", prompt);
    if (!fgets(buf, (int)size, stdin)) return 0;
    buf[strcspn(buf, "\n")] = '\0';
    return 1;
}

/* Reads a non-empty line. */
static int read_text(const char *prompt, char *buf, size_t size) {
    for (;;) {
        if (!read_line(prompt, buf, size)) return 0;
        if (buf[0] != '\0') return 1;
        printf("  Input cannot be empty.\n");
    }
}

/* Reads an integer in [lo, hi]. Returns -1 on end of input. */
static int read_int(const char *prompt, int lo, int hi) {
    char line[64];
    for (;;) {
        if (!read_line(prompt, line, sizeof line)) return -1;
        char *end;
        long v = strtol(line, &end, 10);
        if (end != line && *end == '\0' && v >= lo && v <= hi) return (int)v;
        printf("  Please enter a number from %d to %d.\n", lo, hi);
    }
}

/* Case-insensitive substring check. */
static int contains_ci(const char *hay, const char *needle) {
    char a[256], b[256];
    size_t i;
    for (i = 0; hay[i] && i < sizeof a - 1; i++)    a[i] = (char)tolower((unsigned char)hay[i]);
    a[i] = '\0';
    for (i = 0; needle[i] && i < sizeof b - 1; i++) b[i] = (char)tolower((unsigned char)needle[i]);
    b[i] = '\0';
    return strstr(a, b) != NULL;
}

/* ---------- seat helpers ---------- */

/* Parses a seat like "5C" into zero-based row and column. */
static int parse_seat(const char *s, int *row, int *col) {
    size_t len = strlen(s);
    if (len < 2) return 0;
    for (size_t i = 0; i < len - 1; i++) {
        if (!isdigit((unsigned char)s[i])) return 0;
    }
    if (!isalpha((unsigned char)s[len - 1])) return 0;

    int r = atoi(s) - 1;
    int c = toupper((unsigned char)s[len - 1]) - 'A';
    if (r < 0 || r >= ROWS || c < 0 || c >= COLS) return 0;

    *row = r;
    *col = c;
    return 1;
}

static void seat_label(int row, int col, char *buf) {
    sprintf(buf, "%d%c", row + 1, 'A' + col);
}

static const char *seat_type(int col) {
    return (col == 0 || col == 3) ? "window" : "aisle";
}

static const char *row_zone(int row) {
    if (row < 3)        return "front";
    if (row >= ROWS - 3) return "back";
    return "middle";
}

static int free_seats(const struct Bus *b) {
    int n = 0;
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            if (b->seats[r][c] == 0) n++;
    return n;
}

/* ---------- seat recommendation ---------- */

static int cmp_candidate(const void *a, const void *b) {
    return ((const Candidate *)a)->score - ((const Candidate *)b)->score;
}

/*
 * Scores each free seat:
 *   +10 if it does not match the window/aisle preference
 *   +2 for every row away from the preferred row zone
 * Fills 'out' with the best seats (lowest score first) and returns how many.
 *   pref:    1 window, 2 aisle, 3 no preference
 *   rowpref: 1 front,  2 middle, 3 back, 4 no preference
 */
static int recommend(const struct Bus *b, int pref, int rowpref,
                     Candidate *out, int max_out) {
    Candidate all[ROWS * COLS];
    int n = 0;

    int target = (rowpref == 1) ? 0
               : (rowpref == 2) ? ROWS / 2
               : (rowpref == 3) ? ROWS - 1
               : -1;

    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            if (b->seats[r][c] != 0) continue;

            int is_window = (c == 0 || c == 3);
            int score = 0;
            if (pref == 1 && !is_window) score += 10;
            if (pref == 2 &&  is_window) score += 10;
            if (target >= 0) score += 2 * abs(r - target);

            all[n].row = r;
            all[n].col = c;
            all[n].score = score;
            n++;
        }
    }

    qsort(all, n, sizeof all[0], cmp_candidate);

    int k = (n < max_out) ? n : max_out;
    for (int i = 0; i < k; i++) out[i] = all[i];
    return k;
}

/* ---------- data setup ---------- */

static void add_bus(const char *route, const char *depart, int fare) {
    struct Bus *b = &buses[bus_count];
    b->id = 101 + bus_count;
    strncpy(b->route, route, sizeof b->route - 1);
    b->route[sizeof b->route - 1] = '\0';
    strncpy(b->depart, depart, sizeof b->depart - 1);
    b->depart[sizeof b->depart - 1] = '\0';
    b->fare = fare;
    memset(b->seats, 0, sizeof b->seats);
    bus_count++;
}

static void init_data(void) {
    add_bus("Kharar-Chandigarh",     "08:00", 150);
    add_bus("sector 17-sukna lake",   "10:30", 120);
    add_bus("kharar-chandigarh university",   "14:15", 80);
}

/* ---------- display ---------- */

static void show_buses(void) {
    printf("\n%-5s %-24s %-7s %-6s %s\n", "No.", "Route", "Departs", "Fare", "Free seats");
    for (int i = 0; i < bus_count; i++) {
        printf("%-5d %-24s %-7s Rs %-4d %d\n",
               i + 1, buses[i].route, buses[i].depart,
               buses[i].fare, free_seats(&buses[i]));
    }
}

static void show_seat_map(const struct Bus *b) {
    printf("\nBus %d  %s  (departs %s)\n", b->id, b->route, b->depart);
    printf("    ");
    for (int c = 0; c < COLS; c++) {
        if (c == 2) printf("  ");
        printf(" %c  ", 'A' + c);
    }
    printf("\n");
    for (int r = 0; r < ROWS; r++) {
        printf("%2d  ", r + 1);
        for (int c = 0; c < COLS; c++) {
            if (c == 2) printf("  ");
            printf("[%c] ", b->seats[r][c] ? 'X' : ' ');
        }
        printf("\n");
    }
    printf("\nLegend: [ ] free   [X] booked   Windows: A and D   Aisle: B and C\n");
}

static void print_ticket(const struct Booking *bk) {
    char label[8];
    const struct Bus *b = &buses[bk->bus];
    seat_label(bk->row, bk->col, label);

    printf("\n+------------------------------------+\n");
    printf("|            BUS TICKET              |\n");
    printf("+------------------------------------+\n");
    printf("  Ticket:    %d\n", bk->ticket);
    printf("  Passenger: %s\n", bk->name);
    printf("  Bus:       %d (%s)\n", b->id, b->route);
    printf("  Departs:   %s\n", b->depart);
    printf("  Seat:      %s (%s, %s)\n", label, seat_type(bk->col), row_zone(bk->row));
    printf("  Fare paid: Rs %d\n", bk->fare);
    printf("+------------------------------------+\n");
}

/* ---------- features ---------- */

static void do_view_buses(void) {
    show_buses();
}

static void do_view_map(void) {
    if (bus_count == 0) { printf("No buses available.\n"); return; }
    show_buses();
    int idx = read_int("Choose bus number: ", 1, bus_count);
    if (idx < 0) return;
    show_seat_map(&buses[idx - 1]);
}

static void do_book(void) {
    if (booking_count >= MAX_BOOKINGS) {
        printf("Booking limit reached.\n");
        return;
    }

    show_buses();
    int idx = read_int("Choose bus number: ", 1, bus_count);
    if (idx < 0) return;
    struct Bus *b = &buses[idx - 1];

    if (free_seats(b) == 0) {
        printf("Sorry, this bus is full.\n");
        return;
    }

    char name[NAME_LEN];
    if (!read_text("Passenger name: ", name, sizeof name)) return;

    printf("\nSeat preference:\n");
    printf("  1. Window\n  2. Aisle\n  3. No preference\n");
    int pref = read_int("Choose (1-3): ", 1, 3);
    if (pref < 0) return;

    printf("\nRow preference:\n");
    printf("  1. Front\n  2. Middle\n  3. Back\n  4. No preference\n");
    int rowpref = read_int("Choose (1-4): ", 1, 4);
    if (rowpref < 0) return;

    Candidate top[3];
    int k = recommend(b, pref, rowpref, top, 3);

    printf("\nRecommended seats:\n");
    for (int i = 0; i < k; i++) {
        char label[8];
        seat_label(top[i].row, top[i].col, label);
        printf("  %d. %-4s %-7s %-7s Rs %d\n", i + 1, label,
               seat_type(top[i].col), row_zone(top[i].row), b->fare);
    }

    int choice = read_int("Pick a recommendation (1-3), or 0 to choose your own seat: ", 0, k);
    if (choice < 0) return;

    int r, c;
    if (choice == 0) {
        char buf[16];
        for (;;) {
            if (!read_line("Enter seat (e.g. 5C): ", buf, sizeof buf)) return;
            if (!parse_seat(buf, &r, &c)) {
                printf("  Invalid seat. Use a row from 1 to %d and a letter from A to %c.\n",
                       ROWS, 'A' + COLS - 1);
                continue;
            }
            if (b->seats[r][c] != 0) {
                printf("  That seat is already booked.\n");
                continue;
            }
            break;
        }
    } else {
        r = top[choice - 1].row;
        c = top[choice - 1].col;
    }

    int ticket = next_ticket++;
    b->seats[r][c] = ticket;

    struct Booking *bk = &bookings[booking_count++];
    bk->ticket = ticket;
    strncpy(bk->name, name, NAME_LEN - 1);
    bk->name[NAME_LEN - 1] = '\0';
    bk->bus = (int)(b - buses);
    bk->row = r;
    bk->col = c;
    bk->fare = b->fare;
    bk->active = 1;

    print_ticket(bk);
}

static void do_cancel(void) {
    int ticket = read_int("Ticket ID to cancel: ", 1001, 1000000);
    if (ticket < 0) return;

    struct Booking *bk = NULL;
    for (int i = 0; i < booking_count; i++) {
        if (bookings[i].active && bookings[i].ticket == ticket) {
            bk = &bookings[i];
            break;
        }
    }
    if (!bk) {
        printf("No active booking with that ticket ID.\n");
        return;
    }

    char label[8], answer[8];
    seat_label(bk->row, bk->col, label);
    printf("Cancel ticket %d for %s, seat %s on bus %d?\n",
           bk->ticket, bk->name, label, buses[bk->bus].id);
    if (!read_line("Confirm (y/n): ", answer, sizeof answer)) return;
    if (answer[0] != 'y' && answer[0] != 'Y') {
        printf("Cancellation aborted.\n");
        return;
    }

    buses[bk->bus].seats[bk->row][bk->col] = 0;
    bk->active = 0;

    int refund = bk->fare * 80 / 100;   /* 20% cancellation fee */
    printf("Ticket cancelled. Refund: Rs %d (Rs %d cancellation fee)\n",
           refund, bk->fare - refund);
}

static void do_search(void) {
    char query[NAME_LEN];
    if (!read_text("Search passenger name: ", query, sizeof query)) return;

    int found = 0;
    for (int i = 0; i < booking_count; i++) {
        struct Booking *bk = &bookings[i];
        if (!bk->active || !contains_ci(bk->name, query)) continue;

        char label[8];
        seat_label(bk->row, bk->col, label);
        printf("  Ticket %d | %-15s | Bus %d | Seat %-4s | Rs %d\n",
               bk->ticket, bk->name, buses[bk->bus].id, label, bk->fare);
        found++;
    }
    if (!found) printf("No active bookings found for \"%s\".\n", query);
}

/* ---------- menu ---------- */

static void print_menu(void) {
    printf("\n========== BUS TICKET SYSTEM ==========\n");
    printf(" 1. View buses\n");
    printf(" 2. View seat map\n");
    printf(" 3. Book a ticket (with seat recommendation)\n");
    printf(" 4. Cancel a ticket\n");
    printf(" 5. Search bookings by passenger name\n");
    printf(" 0. Exit\n");
    printf("=======================================\n");
}

int main(void) {
    init_data();
    printf("Welcome to the Bus Ticket System!\n");

    for (;;) {
        print_menu();
        int choice = read_int("Choose an option: ", 0, 5);
        if (choice <= 0) break;     /* 0 = exit, -1 = end of input */

        switch (choice) {
            case 1: do_view_buses(); break;
            case 2: do_view_map();   break;
            case 3: do_book();       break;
            case 4: do_cancel();     break;
            case 5: do_search();     break;
        }
    }

    printf("Thank you for travelling with us!\n");
    return 0;
}
