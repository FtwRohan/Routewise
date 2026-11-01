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

Technologies Used
Language: C (C99 or later)
Compiler: GCC
Libraries: stdio.h, stdlib.h, string.h, ctype.h
No external dependencies
