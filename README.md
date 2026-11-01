# seat_oracle
This project is a command-line Bus Ticket System written in C. It lets passengers view buses and seat maps, book tickets, cancel bookings, and search passengers by name.

SeatSmart is a command-line bus ticket system written in C that makes booking a seat simple and efficient. Instead of making passengers scan the whole bus for a seat, it asks for their window or aisle and front, middle, or back preferences, then recommends the best available options. The project combines structs, 2D arrays, and modular functions to handle the full booking process, from seat selection and ticketing to cancellation and passenger search, all using only the standard C library.
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>SeatSmart: Bus Ticket System in C</title>
  <style>
    body {
      font-family: Arial, Helvetica, sans-serif;
      line-height: 1.6;
      color: #222;
      max-width: 800px;
      margin: 40px auto;
      padding: 0 20px;
    }
    h1 {
      color: #1a4f8b;
      border-bottom: 3px solid #1a4f8b;
      padding-bottom: 8px;
    }
    h2 {
      color: #1a4f8b;
      margin-top: 28px;
    }
    p.intro {
      font-size: 1.05em;
      background: #f2f6fb;
      border-left: 4px solid #1a4f8b;
      padding: 12px 16px;
    }
    ul {
      padding-left: 22px;
    }
    li {
      margin-bottom: 6px;
    }
    code {
      background: #eef1f4;
      padding: 2px 6px;
      border-radius: 4px;
      font-family: Consolas, monospace;
    }
  </style>
</head>
<body>

  <h1>SeatSmart: Bus Ticket System in C</h1>

  <p class="intro">
    SeatSmart is a command-line bus ticket system written in C that makes booking a seat simple and efficient.
    Instead of making passengers scan the whole bus for a seat, it asks for their window or aisle and front,
    middle, or back preferences, then recommends the best available options. The project combines structs,
    2D arrays, and modular functions to handle the full booking process, from seat selection and ticketing to
    cancellation and passenger search, all using only the standard C library.
  </p>

  <h2>Overview</h2>
  <ul>
    <li>A command-line bus ticket system written in C</li>
    <li>Lets passengers view schedules, choose seats, book, cancel, and search bookings by name</li>
    <li>Built with structs, arrays, and modular functions, using only the standard library</li>
  </ul>

  <h2>Bus and Seat Layout</h2>
  <ul>
    <li>Manages up to four buses, each with a route, departure time, and fare</li>
    <li>Each bus has 10 rows with 4 seats per row</li>
    <li>Seats A and D are window seats; seats B and C are aisle seats</li>
    <li>Seat data is stored in a 2D array: <code>0</code> means free, and a non-zero value is the booked ticket ID</li>
  </ul>

  <h2>Seat Recommendation Engine</h2>
  <ul>
    <li>Passenger states a window or aisle preference and a front, middle, or back preference</li>
    <li>Every free seat is scored (lower is better):
      <ul>
        <li>Penalty for not matching the window or aisle preference</li>
        <li>Smaller penalty for each row away from the preferred zone</li>
      </ul>
    </li>
    <li>Scored seats are sorted with <code>qsort</code></li>
    <li>The top three matches are offered, or the passenger can choose a seat manually (e.g. <code>5C</code>)</li>
  </ul>

  <h2>Booking and Cancellation</h2>
  <ul>
    <li>Manual seat entries are checked for format, range, and availability</li>
    <li>Confirmed bookings print a formatted ticket with passenger, bus, seat, and fare details</li>
    <li>Cancellation asks for confirmation, frees the seat, and applies a 20% fee</li>
    <li>Cancelled bookings are marked inactive rather than deleted, preserving history</li>
    <li>Passenger search by name is case-insensitive</li>
  </ul>

  <h2>Input Handling</h2>
  <ul>
    <li>All keyboard input is read with <code>fgets</code> to avoid <code>scanf</code> buffer issues</li>
    <li>Helper functions validate numbers, text, and seat codes before use</li>
    <li>Keeps the main logic simple and the system reliable</li>
  </ul>

  <h2>Technical Details</h2>
  <ul>
    <li>Language: C (C99 or later)</li>
    <li>Compiler: GCC</li>
    <li>Libraries: <code>stdio.h</code>, <code>stdlib.h</code>, <code>string.h</code>, <code>ctype.h</code></li>
    <li>No external dependencies</li>
  </ul>

  <h2>Limitations and Future Work</h2>
  <ul>
    <li>Data is stored in memory and resets when the program closes</li>
    <li>Planned: file storage using <code>fwrite</code> and <code>fread</code></li>
    <li>Planned: waitlist using a queue</li>
    <li>Planned: time-based refund rules</li>
  </ul>

</body>
</html>
<img width="424" height="355" alt="Screenshot 2026-11-01 at 7 13 03 PM" src="https://github.com/user-attachments/assets/8dede0cd-a93e-4605-92e5-78eec9226d52" />
<img width="511" height="292" alt="Screenshot 2026-11-01 at 7 13 18 PM" src="https://github.com/user-attachments/assets/df378264-2005-4a9c-8770-c8aa71119ec5" />

<img width="522" height="641" alt="Screenshot 2026-11-01 at 7 14 00 PM" src="https://github.com/user-attachments/assets/d4dc834d-3533-441f-90f6-8db3664a5b1a" />
<img width="517" height="400" alt="Screenshot 2026-11-01 at 7 14 12 PM" src="https://github.com/user-attachments/assets/2437d728-e521-46b1-a3a4-2fc10390845d" />
