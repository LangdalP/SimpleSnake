#include <SDL.h>
#include <iostream>
#include "SnakeEngine.h"
#include <vector>

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1200;

//const int SCREEN_WIDTH = 1366;
//const int SCREEN_HEIGHT = 768;

const int SNAKE_GRID_WIDTH = BOARD_WIDTH;
const int SNAKE_GRID_HEIGHT = BOARD_HEIGHT;

const int SNAKE_CELL_W_PX = SCREEN_WIDTH/SNAKE_GRID_WIDTH;
const int SNAKE_CELL_H_PX = SCREEN_HEIGHT/SNAKE_GRID_HEIGHT;

// Fixed game logic step
const Uint32 TICK_STEP = 60; // ms

void updateRects(std::vector<SDL_Rect>* rectVec, std::vector<SnakeEngine::BoardPos*>* snakeVec)
{
	int numRects = rectVec->size();
	int numSnakeSquares = snakeVec->size();

	// Just fill rectVec with sufficient rectangles at first
	while (numSnakeSquares > numRects) {
		SDL_Rect fillerRect;
		fillerRect.w = SNAKE_CELL_W_PX;
		fillerRect.h = SNAKE_CELL_H_PX;

		rectVec->insert(rectVec->begin(), fillerRect);
		numRects = rectVec->size();
	}

	int rectVecIndex = 0;
	for (auto& pos : *snakeVec) {
		int gridX = pos->x;
		int gridY = pos->y;

		int screenX = gridX*SNAKE_CELL_W_PX;
		int screenY = gridY*SNAKE_CELL_H_PX;

		rectVec->at(rectVecIndex).x = screenX;
		rectVec->at(rectVecIndex).y = screenY;

		rectVecIndex++;
	}
}

int main(int arc, char* argv[])
{
	// Create SnakeEngine object, which is responsible for all game logic
	SnakeEngine engine(SnakeEngine::NORTH);

	///////////////////////////////////////////////////////
	// Init a bunch of SDL stuff (graphics and user input)
	///////////////////////////////////////////////////////

	// INIT engine
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		std::cout << SDL_GetError() << std::endl;
		return 1;
	}

	// Make the window
	SDL_Window *screen = nullptr;
	screen = SDL_CreateWindow("MaxGFX Extreme", 
		0,
		0,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_FULLSCREEN);

	if (screen == nullptr) {
		std::cout << SDL_GetError() << std::endl;
		return 1;
	}

	// Make the renderer
	SDL_Renderer *renderer = nullptr;
	renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr) {
		std::cout << SDL_GetError() << std::endl;
		return 1;
	}

	// Make event object to receive input events
	SDL_Event event;

	// Set background to black
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	// Create a vector for holding all SDL_Rects needed to draw snake
	std::vector<SDL_Rect> snakeRects;

	// Fill it
	updateRects(&snakeRects, engine.getSnakeVector());

	// Create a rect for an apple
	SDL_Rect appleRect;
	appleRect.w = SNAKE_CELL_W_PX;
	appleRect.h = SNAKE_CELL_H_PX;
	appleRect.x = engine.getAppleX()*SNAKE_CELL_W_PX;
	appleRect.y = engine.getAppleY()*SNAKE_CELL_H_PX;

	// Remove the cursor and set a delay for the startup
	SDL_ShowCursor(0);
	SDL_Delay(500);

	///////////////////////////////////////////////////////
	// GAME LOOP
	///////////////////////////////////////////////////////
	bool running = true;
	Uint32 lastTick = SDL_GetTicks()-100;
	bool doTick = true;
	while (running)
	{
		// Checks for input events, such as clicking "X"/close on the window or keyboard input
		if (SDL_PollEvent(&event)) 
		{
			if (event.type == SDL_QUIT)
			{
				running = false;
			}
			if (event.type == SDL_KEYDOWN)
			{
				switch(event.key.keysym.sym) {
				case SDLK_ESCAPE:
					running = false;
					break;
				case SDLK_w:
					engine.setDirection(SnakeEngine::NORTH);
					break;
				case SDLK_a:
					engine.setDirection(SnakeEngine::WEST);
					break;
				case SDLK_s:
					engine.setDirection(SnakeEngine::SOUTH);
					break;
				case SDLK_d:
					engine.setDirection(SnakeEngine::EAST);
					break;
				}

			}

		}
		
		// Game logic
		if (!doTick) {
			if ((SDL_GetTicks() - lastTick) > TICK_STEP) {
				doTick = true;
				lastTick = SDL_GetTicks();
			}
		}

		if (doTick) {
			engine.tick();
			doTick = false;

			if (engine.hasCrashed()) {
				running = false;
				SDL_Delay(1000);
			}
		}

		// Rendering
		updateRects(&snakeRects, engine.getSnakeVector());

		appleRect.x = engine.getAppleX()*SNAKE_CELL_W_PX;
		appleRect.y = engine.getAppleY()*SNAKE_CELL_H_PX;

		// Set background to black and clear
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// Draw snake
		SDL_SetRenderDrawColor(renderer, 12, 172, 12, 255);
		for (auto& rect : snakeRects) {
			SDL_RenderFillRect(renderer, &rect);
		}

		// Draw an apple
		SDL_SetRenderDrawColor(renderer, 202, 3, 3, 255);
		SDL_RenderFillRect(renderer, &appleRect);

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(screen);

	SDL_Quit();

	return 0;
}