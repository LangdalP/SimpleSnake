
#include "SnakeEngine.h"
#include <stdlib.h>	// rand, srand
#include <time.h>	// time for seed
#include <iostream>	// cout

SnakeEngine::SnakeEngine(MOVE_DIRECTION startDirection)
{
	// Fill array with zeros = empty squares
	for (auto& subArray : board)
	{
		for (auto& square : subArray)
		{
			square = 0;
		}
	}

	dir = startDirection;
	nextDir = startDirection;
	crashed = false;

	BoardPos* head = new BoardPos;
	BoardPos* tail = new BoardPos;

	if (dir == NORTH) {
		head->x = BOARD_WIDTH/2;
		head->y = BOARD_HEIGHT/2;
		tail->x = BOARD_WIDTH/2;
		tail->y = BOARD_HEIGHT/2+1;
	} else if (dir == SOUTH) {
		tail->x = BOARD_WIDTH/2;
		tail->y = BOARD_HEIGHT/2;
		head->x = BOARD_WIDTH/2;
		head->y = BOARD_HEIGHT/2+1;
	} else if (dir == WEST) {
		head->x = BOARD_WIDTH/2;
		head->y = BOARD_HEIGHT/2;
		tail->x = BOARD_WIDTH/2+1;
		tail->y = BOARD_HEIGHT/2;
	} else {
		head->x = BOARD_WIDTH/2+1;
		head->y = BOARD_HEIGHT/2;
		tail->x = BOARD_WIDTH/2;
		tail->y = BOARD_HEIGHT/2;
	}
	snakeVec.insert(snakeVec.begin(), head);
	snakeVec.insert(snakeVec.end(), tail);

	srand (time(NULL));
	spawnApple();
}


SnakeEngine::~SnakeEngine(void)
{
	// Because we are dynamically allocating new BoardPos structs we must also delete them upon object destruction
	for (auto& pos : snakeVec) {
		delete pos;
	}
}

void SnakeEngine::moveSnake(int nextX, int nextY)
{
	int frontTempX = 0;
	int frontTempY = 0;
	BoardPos* headPos = snakeVec[0];

	frontTempX = headPos->x;
	frontTempY = headPos->y;

	// Setting head to new coordinate
	headPos->x = nextX;
	headPos->y = nextY;

	// Looping through rest of snake, moving each snake-sqaure one forward
	int index = 0;
	for (auto& pos : snakeVec) {
		if (index == 0) {
			index++;
			continue;
		}

		int tempx = pos->x;
		int tempy = pos->y;
		pos->x = frontTempX;
		pos->y = frontTempY;
		frontTempX = tempx;
		frontTempY = tempy;

		index++;
	}
}

void SnakeEngine::spawnApple(void)
{
	int x, y;
	while (true) {
		x = rand() % BOARD_WIDTH;
		y = rand() % BOARD_HEIGHT;

		if (snakeIntersects(x, y)) {
			continue;
		} else {
			board[x][y] = 1;
			appleX = x;
			appleY = y;
			return;
		}
	}
}

bool SnakeEngine::snakeIntersects(int x, int y)
{
	for (auto& pos : snakeVec) {
		if (x == pos->x && y == pos->y) {
			return true;
		}
	}
	return false;
}

void SnakeEngine::eatApple(int x, int y)
{
	if (board[x][y] != 1) {
		return;
	}

	// Apple is removed from the board
	board[x][y] = 0;

	// Snake is grown and moved
	growSnake(x, y);

	// New apple is spawned
	spawnApple();
}

void SnakeEngine::growSnake(int x, int y)
{
	BoardPos* newHead = new BoardPos;
	newHead->x = x;
	newHead->y = y;
	snakeVec.insert(snakeVec.begin(), newHead);
}

bool SnakeEngine::posIsOutsideBoard(int x, int y)
{
	if (x < 0 || x >= BOARD_WIDTH) {
		return true;
	} else if (y < 0 || y >= BOARD_HEIGHT) {
		return true;
	} else {
		return false;
	}
}

std::array<std::array<int, BOARD_HEIGHT>, BOARD_WIDTH>* SnakeEngine::getBoardArray(void)
{
	return &board;
}

int& SnakeEngine::getSquare(int x, int y)
{
	return (board[x][y]);
}

int SnakeEngine::getAppleX(void)
{
	return appleX;
}

int SnakeEngine::getAppleY(void)
{
	return appleY;
}

bool SnakeEngine::setDirection(MOVE_DIRECTION newDir)
{
	if (dir == NORTH || dir == SOUTH) {
		if  (newDir == EAST || newDir == WEST) {
			nextDir = newDir;
			return true;
		} else {
			return false;
		}
	} else if (dir == WEST || dir == EAST) {
		if  (newDir == NORTH || newDir == SOUTH) {
			nextDir = newDir;
			return true;
		} else {
			return false;
		}
	}
	return false;
}

void SnakeEngine::tick(void)
{
	dir = nextDir;

	int nextX;
	int nextY;
	BoardPos* headPos = snakeVec[0];

	if (dir == NORTH) {
		nextX = headPos->x;
		nextY = headPos->y - 1;

		if (posIsOutsideBoard(nextX, nextY)) {
			nextY = BOARD_HEIGHT-1;
		}
	} else if (dir == SOUTH) {
		nextX = headPos->x;
		nextY = headPos->y + 1;

		if (posIsOutsideBoard(nextX, nextY)) {
			nextY = 0;
		}
	} else if (dir == WEST) {
		nextX = headPos->x - 1;
		nextY = headPos->y;

		if (posIsOutsideBoard(nextX, nextY)) {
			nextX = BOARD_WIDTH - 1;
		}
	} else { // EAST
		nextX = headPos->x + 1;
		nextY = headPos->y;

		if (posIsOutsideBoard(nextX, nextY)) {
			nextX = 0;
		}
	}

	if (snakeIntersects(nextX, nextY)) {
		crashed = true;
	} else if (board[nextX][nextY] == 1) {
		eatApple(nextX, nextY);
	} else {
		moveSnake(nextX, nextY);
	}
}

bool SnakeEngine::hasCrashed(void)
{
	return crashed;
}

std::string SnakeEngine::boardToString(void)
{
	std::string boardStr = "";
	for (int y = 0; y<BOARD_HEIGHT; y++) {
		for (int x = 0; x<BOARD_WIDTH; x++) {
			if (snakeIntersects(x, y)) {
				boardStr += "X";

			} else {
				if (board[x][y] == 0) {
					boardStr += "0";
				} else if (board[x][y] == 1) {
					boardStr += "1";
				}
			}
		}
		boardStr += "\n";
	}
	return boardStr;
}


std::string SnakeEngine::snakeToString(void)
{
	std::string snakeStr = "";
	for (auto& pos : snakeVec) {
		snakeStr += std::to_string(pos->x);
		snakeStr += ", ";
		snakeStr += std::to_string(pos->y);
		snakeStr += " | ";
	}
	return snakeStr;
}


std::vector<SnakeEngine::BoardPos*>* SnakeEngine::getSnakeVector(void)
{
	return &snakeVec;
}

