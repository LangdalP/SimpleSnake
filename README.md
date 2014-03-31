SimpleSnake
===========

I am currently learning game programming with C++, and I made a minimal snake game with SDL for graphics and user input.

31.03.2014:
The snake engine is responsible for game logic and the implementation is currently not optimal. The array used to represent the board can be removed (requires little refactoring), as I'm storing the actual snake in a vector (because it grows and I want to know where the head is, currently 0th item in the vector), and I am storing apple positions both in the array and in member variables appleX and appleY.
