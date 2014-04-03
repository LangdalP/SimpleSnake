#include <SDL.h>            // Graphics and user input
#include <SDL_ttf.h>        // Drawing text

#include <iostream>         // cout
#include "SnakeEngine.h"    // snake logic
#include <vector>           // vector
#include <string>           // to_string()

//const int SCREEN_WIDTH = 1920;
//const int SCREEN_HEIGHT = 1080;

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1200;

//const int SCREEN_WIDTH = 1366;
//const int SCREEN_HEIGHT = 768;

const int SNAKE_GRID_WIDTH = BOARD_WIDTH;
const int SNAKE_GRID_HEIGHT = BOARD_HEIGHT;

const int SNAKE_CELL_W_PX = SCREEN_WIDTH/SNAKE_GRID_WIDTH;
const int SNAKE_CELL_H_PX = SCREEN_HEIGHT/SNAKE_GRID_HEIGHT;

// Fixed game logic step
const Uint32 TICK_STEP = 50; // ms

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

SDL_Texture* createTextTexture(SDL_Renderer* renderer, std::string message, TTF_Font* font, SDL_Color color) {
    SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);

    //Cleanup and return
    SDL_FreeSurface(surf);
    return texture;
}

void updateTextRect(SDL_Rect* rect, SDL_Texture* texture) {
    int textWidth = 0;
    int textHeight = 0;
    SDL_QueryTexture(texture, NULL, NULL, &textWidth, &textHeight);

    rect->w = textWidth;
    rect->h = textHeight;
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
		SDL_Quit();
		return 1;
	}

	// Make the renderer
	SDL_Renderer *renderer = nullptr;
	renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr) {
		std::cout << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

    // TODO remove

	// Initialize SDL_ttf library
	if (TTF_Init() != 0) {
        std::cout << TTF_GetError() << std::endl;
        SDL_Quit();
        return 1;
	}

	// Load a font
	TTF_Font *font = nullptr;

	font = TTF_OpenFont("airstrike.ttf", 60);
	if (font == nullptr) {
        std::cout << TTF_GetError() << std::endl;
        SDL_Quit();
        return 1;
	}

	SDL_Color textColor = {255, 20, 147};

    // Create a texture to hold text
    SDL_Texture* textTexture = nullptr;
    textTexture = createTextTexture(renderer, "2", font, textColor);

    // Rect to hold text position and dimension
    SDL_Rect textPosRect;
    textPosRect.x = 10;
    textPosRect.y = 5;
    textPosRect.w = 0;
    textPosRect.h = 0;

    // Update the rect
    updateTextRect(&textPosRect, textTexture);

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
	size_t snakeSize = snakeRects.size();
	bool snakeGrew = false;

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
				case SDLK_UP:
					engine.setDirection(SnakeEngine::NORTH);
					break;
				case SDLK_LEFT:
					engine.setDirection(SnakeEngine::WEST);
					break;
				case SDLK_DOWN:
					engine.setDirection(SnakeEngine::SOUTH);
					break;
				case SDLK_RIGHT:
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
				SDL_Delay(5000);
			}
		}

		// Did the snake grow?
		if (snakeRects.size() > snakeSize) {
            snakeSize = snakeRects.size();
            snakeGrew = true;
		} else {
            snakeGrew = false;
		}

		// Rendering
		updateRects(&snakeRects, engine.getSnakeVector());

		appleRect.x = engine.getAppleX()*SNAKE_CELL_W_PX;
		appleRect.y = engine.getAppleY()*SNAKE_CELL_H_PX;

		// Set background to black and clear
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// Draw snake

		bool snakeHead = true;
		for (auto& rect : snakeRects) {
            if (snakeHead) {
                SDL_SetRenderDrawColor(renderer, 12, 172, 12, 255);
                SDL_RenderFillRect(renderer, &rect);
                snakeHead = false;
            } else {
                SDL_SetRenderDrawColor(renderer, 12, 100, 12, 255);
                SDL_RenderFillRect(renderer, &rect);
            }
		}

		// Draw an apple
		SDL_SetRenderDrawColor(renderer, 202, 3, 3, 255);
		SDL_RenderFillRect(renderer, &appleRect);

		// Draw text
		if (snakeGrew) {
            SDL_DestroyTexture(textTexture);
            textTexture = createTextTexture(renderer, std::to_string(snakeSize), font, textColor);
            updateTextRect(&textPosRect, textTexture);
		}

		SDL_RenderCopy(renderer, textTexture, NULL, &textPosRect);

		SDL_RenderPresent(renderer);
	}

    // Cleanup
    SDL_DestroyTexture(textTexture);

    TTF_CloseFont(font);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(screen);

	SDL_Quit();

	return 0;
}
