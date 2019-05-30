/* this file is the implementation of the Game class in Game.h */

#include "Game.h";
#include <iostream>
const int thickness = 15;
const float paddleH = 100.0f;

// init the gameWindow to be null pointer and the game is running
Game::Game(void) 
	: gameWindow(nullptr), renderer(nullptr), isRunning(true),
	ticksCount(0), paddleDir(0){}

// Initialize the game environment
bool Game::Init(void) {

	int sdlResult = SDL_Init(SDL_INIT_VIDEO); // init SDL video
	if (sdlResult != 0) { // if SDL init fail
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError);
		return false;
	}

	// create new Window
	gameWindow = SDL_CreateWindow(
		"first game" , // window titile
		100,  // top left x coordinate
		100,  // top left y coordinate
		1024, // Width of window
		768,  // Height of window
		0     // Flags (0 is no flags set)
	);

	// check if the window is created
	if (!gameWindow) {
		SDL_Log("Fail to create window: %s", SDL_GetError());
		return false;
	}

	renderer = SDL_CreateRenderer(
		gameWindow, // The window where image is rendered
		-1, // index, usually -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	// renderer is not successfully created
	if (!renderer) {
		SDL_Log("Fail to create renderer");
		return false;
	}

	// Initial position of the ball and the paddle
	paddlePos.x = 10.0f;
	paddlePos.y = 768.0f/2.0f;
	ballPos.x = 1024.0f/2.0f;
	ballPos.y = 768.0f/2.0f;

	//  Inital velocity of the ball
	ballVel.x = -200.0f;
	ballVel.y = 235.0f;

	return true;
}

void Game::ShutDown(void) {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(gameWindow);
	SDL_Quit();
}

void Game::RunLoop(void) {
	while (isRunning) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput(void) {
	SDL_Event event;

	// While there are events in the queue
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			// Handle each event type here
		case SDL_QUIT:
			isRunning = false;
			break;
		}
	}

	//Get state of keyboard
	const Uint8 *state = SDL_GetKeyboardState(NULL);

	// if escape button is pressed
	if (state[SDL_SCANCODE_ESCAPE]) {
		isRunning = false;
	}

	paddleDir = 0;
	if (state[SDL_SCANCODE_W]) {
		paddleDir -= 1;
	}
	if (state[SDL_SCANCODE_S]) {
		paddleDir += 1;
	}
}
void Game::UpdateGame(void) {

	//Wait until 16ms has passed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticksCount + 16));

	// Note: SDL_GetTicks() return the amount of ticks (1/1000 seconds) since SDL_Init()
	// delta time is the difference in ticks from last frame
	// it is converted to second
	float deltaTime = (SDL_GetTicks() - ticksCount) / 1000.0f;

	// Update ticksCount for next frame
	ticksCount = SDL_GetTicks();

	// Set maximum delta time value
	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}
	
	if (paddleDir != 0) {
		paddlePos.y += paddleDir * 300.0f * deltaTime;
	}

	// Make sure the paddle does not go off screen by setting boundary
	if (paddlePos.y < (paddleH / 2.0f + thickness)) {
		paddlePos.y = paddleH / 2.0f + thickness;
	}

	if (paddlePos.y > (768.0f - paddleH / 2.0f - thickness)) {
		paddlePos.y = 768.0f - paddleH / 2.0f - thickness;
	}

	// Moving the ball
	ballPos.x += ballVel.x * deltaTime;
	ballPos.y += ballVel.y * deltaTime;

	// Did the ball collide with top wall?
	if (ballPos.y <= thickness && ballVel.y < 0.0f) {
		ballVel.y *= -1;
	}

	// Did the ball collide with bottom wall?
	if (ballPos.y >= (768.0f - thickness) && ballVel.y > 0.0f) {
		ballVel.y *= -1;
	}

	// Interact with the paddle
	// difference between the y coordinate of the paddle and the ball
	float diff = ballPos.y - paddlePos.y;
	// Tack the absolute value
	diff = (diff >= 0) ? diff : -diff;
	if (
		// Our y - difference is small enough
		diff <= paddleH / 2.0f &&
		// We are at the correct x postion
		ballPos.x <= 25.0f && ballPos.x >= 20.0f &&
		// The ball is moving to the left
		ballVel.x < 0.0f
		) {
		ballVel.x *= -1;
	}
	// You cannot blocked the ball and it goes off screen
	else if (ballPos.x <= 0.0f) {
		isRunning = 0;
	}
	// Did the ball collid with right wall?
	else if (ballPos.x >= (1024.0f - thickness) && ballVel.x > 0.0f) {
		ballVel.x *= -1;
	}
}

void Game::GenerateOutput(void) {
	SDL_SetRenderDrawColor(
		renderer,
		0,   // Red
		0,   // Green
		255, // Blue
		255  // Alpha
	);

	// Clear the back buffer
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	// Draw top wall
	SDL_Rect wall{
		0, // x top left corner
		0, // y top left corner
		1024, // width
		thickness, // height
	};
	SDL_RenderFillRect(renderer, &wall);

	// Draw bottom wall
	wall.y = 768 - thickness;
	SDL_RenderFillRect(renderer, &wall);

	// Draw right wall
	wall.x = 1024 - thickness;
	wall.y = 0;
	wall.w = thickness;
	wall.h = 768;
	SDL_RenderFillRect(renderer, &wall);

	// Draw the ball
	SDL_Rect ball{
		static_cast<int> (ballPos.x - thickness / 2),
		static_cast<int> (ballPos.y - thickness / 2),
		thickness, thickness
	};
	SDL_RenderFillRect(renderer, &ball);

	// Draw the paddle
	SDL_Rect paddle{
		static_cast<int> (paddlePos.x),
		static_cast<int> (paddlePos.y - paddleH / 2),
		thickness,
		static_cast<int> (paddleH)
	};
	SDL_RenderFillRect(renderer, &paddle);
	// Swap buffer
	SDL_RenderPresent(renderer);
}