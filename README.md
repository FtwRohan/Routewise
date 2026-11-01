# seat_oracle
This project is a command-line Bus Ticket System written in C. It lets passengers view buses and seat maps, book tickets, cancel bookings, and search passengers by name.

Bus Ticket System (C)

A command-line bus ticket booking system written in C. It lets passengers view buses, book seats with smart seat recommendations, cancel tickets, and search bookings by passenger name.

Features
View available buses with route, departure time, fare, and free seats
Display a seat map ([X] booked, [ ] free)
Book tickets with seat recommendation based on window/aisle and row preferences
Manual seat selection with validation (invalid or already booked seats are rejected)
Cancel tickets with a 20% cancellation fee and refund calculation
Search bookings by passenger name (case-insensitive)
Input validation for all menu choices and user input
How the Seat Recommender Works

Each free seat is given a score (lower is better):

+10 penalty if the seat does not match the window/aisle preference
+2 penalty for each row away from the preferred zone (front, middle, or back)

The seats are sorted using qsort, and the top three are offered to the passenger.

<img width="424" height="355" alt="Screenshot 2026-11-01 at 7 13 03 PM" src="https://github.com/user-attachments/assets/8dede0cd-a93e-4605-92e5-78eec9226d52" />
<img width="511" height="292" alt="Screenshot 2026-11-01 at 7 13 18 PM" src="https://github.com/user-attachments/assets/df378264-2005-4a9c-8770-c8aa71119ec5" />

<img width="522" height="641" alt="Screenshot 2026-11-01 at 7 14 00 PM" src="https://github.com/user-attachments/assets/d4dc834d-3533-441f-90f6-8db3664a5b1a" />
<img width="517" height="400" alt="Screenshot 2026-11-01 at 7 14 12 PM" src="https://github.com/user-attachments/assets/2437d728-e521-46b1-a3a4-2fc10390845d" />
