# seat_oracle
This project is a command-line Bus Ticket System written in C. It lets passengers view buses and seat maps, book tickets, cancel bookings, and search passengers by name.

SeatSmart is a command-line bus ticket system written in C that makes booking a seat simple and efficient. Instead of making passengers scan the whole bus for a seat, it asks for their window or aisle and front, middle, or back preferences, then recommends the best available options. The project combines structs, 2D arrays, and modular functions to handle the full booking process, from seat selection and ticketing to cancellation and passenger search, all using only the standard C library.
Overview
A command-line bus ticket system written in C
Lets passengers view schedules, choose seats, book, cancel, and search bookings by name
Built with structs, arrays, and modular functions, using only the standard library
Bus and Seat Layout
Manages up to four buses, each with a route, departure time, and fare
Each bus has 10 rows with 4 seats per row
Seats A and D are window seats; seats B and C are aisle seats
Seat data is stored in a 2D array: 0 means free, and a non-zero value is the booked ticket ID
Seat Recommendation Engine
Passenger states a window or aisle preference and a front, middle, or back preference
Every free seat is scored (lower is better):
Penalty for not matching the window or aisle preference
Smaller penalty for each row away from the preferred zone
Scored seats are sorted with qsort
The top three matches are offered, or the passenger can choose a seat manually (e.g. 5C)
Booking and Cancellation
Manual seat entries are checked for format, range, and availability
Confirmed bookings print a formatted ticket with passenger, bus, seat, and fare details
Cancellation asks for confirmation, frees the seat, and applies a 20% fee
Cancelled bookings are marked inactive rather than deleted, preserving history
Passenger search by name is case-insensitive
Input Handling
All keyboard input is read with fgets to avoid scanf buffer issues
Helper functions validate numbers, text, and seat codes before use
Keeps the main logic simple and the system reliable

<img width="424" height="355" alt="Screenshot 2026-11-01 at 7 13 03 PM" src="https://github.com/user-attachments/assets/8dede0cd-a93e-4605-92e5-78eec9226d52" />
<img width="511" height="292" alt="Screenshot 2026-11-01 at 7 13 18 PM" src="https://github.com/user-attachments/assets/df378264-2005-4a9c-8770-c8aa71119ec5" />

<img width="522" height="641" alt="Screenshot 2026-11-01 at 7 14 00 PM" src="https://github.com/user-attachments/assets/d4dc834d-3533-441f-90f6-8db3664a5b1a" />
<img width="517" height="400" alt="Screenshot 2026-11-01 at 7 14 12 PM" src="https://github.com/user-attachments/assets/2437d728-e521-46b1-a3a4-2fc10390845d" />
