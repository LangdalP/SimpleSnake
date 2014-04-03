SimpleSnake
===========

I am currently learning game programming with C++, and I made a minimal snake game with SDL for graphics and user input.

31.03.2014:
The snake engine (class) is responsible for game logic and the implementation is currently not optimal. The array used to represent the board can be removed (requires little refactoring), as I'm storing the actual snake in a vector (because it grows and I want to know where the head is, currently 0th item in the vector), and I am storing apple positions both in the array and in member variables appleX and appleY.

Also I am  dynamically allocating new BoardPos structs for the snake vector (and deleting them in the destructor). Might be unnecessary.

Screenshot: https://dl.dropboxusercontent.com/u/11211513/SimpleSnake%202014-03-31%2022-35-43-14.jpg
