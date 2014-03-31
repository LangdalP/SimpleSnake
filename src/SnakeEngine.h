#pragma once

#ifndef GRANDFATHER_H
#define GRANDFATHER_H

#include <array>
#include <vector>
#include <string>

const int BOARD_WIDTH = 32;
const int BOARD_HEIGHT = 20;

// const int BOARD_WIDTH = 32;
// const int BOARD_HEIGHT = 18;

class SnakeEngine
{
public:
	enum MOVE_DIRECTION { NORTH, WEST, SOUTH, EAST};

	// Struct for a position on the board
	struct BoardPos {
		int x;
		int y;
	};
private:
	// The board
	std::array<std::array<int, BOARD_HEIGHT>, BOARD_WIDTH> board;

	// The current move direction for the head
	MOVE_DIRECTION dir;

	// The desired next direction
	MOVE_DIRECTION nextDir;

	// Boolean for whether the snake has crashed into itself or not
	bool crashed;

	// Vector to hold the squares that the snake is occupying
	std::vector<BoardPos*> snakeVec;

	int appleX;
	int appleY;

	// Method for spawning an apple at a random location except not on the snake. Can spawn apple on apple.
	void spawnApple(void);

	// Method for moving the snake, giving the new (x, y) for the head
	void moveSnake(int x, int y);

	// Method for checking if snake occupies square
	bool snakeIntersects(int x, int y);

	// Method for removing the eaten apple, growing snake, and spawning a new apple
	void eatApple(int x, int y);

	// Method for growing snake, inserting new head at (x, y)
	void growSnake(int x, int y);

	// Method for checking if snake is trying to exit the map
	bool posIsOutsideBoard(int x, int y);

public:
	SnakeEngine(MOVE_DIRECTION startDirection);
	~SnakeEngine(void);

	std::array<std::array<int, BOARD_HEIGHT>, BOARD_WIDTH>* getBoardArray(void);
	int& getSquare(int x, int y);
	std::vector<SnakeEngine::BoardPos*>* getSnakeVector(void);

	int getAppleX(void);
	int getAppleY(void);

	// Set desired move direction, if it is valid. Returns true upon success, and false otherwise
	bool setDirection(MOVE_DIRECTION newDir);

	// Do a game tick, using the current MOVE_DIRECTION
	void tick(void);

	// Returns whether a crash has occurred
	bool hasCrashed(void);

	// Output the entire board + snake as string
	std::string boardToString(void);

	// Prints all squares that the snake is occupying
	std::string snakeToString(void);
};

#endif /* GRANDFATHER_H */