/**
* This is the header file for Game class
* In this header file, the skeleton structure of the game is outlined.
*/
#pragma once
#include <SDL.h>
#include "pch.h"

// 2D Vector struct
struct Vector2 {
	float x;
	float y;
};

class Game {
public:
	Game();

	/* Initialize the game. this method return true if the game is initialized 
	it return false otherwise */
	bool Init();

	// The main game loop
	void RunLoop();

	// Shutdown the game
	void ShutDown();
private:
	// Helper methods used in the game loop
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	// Window created by SDL
	SDL_Window* gameWindow;

	// Renderer for 2D image
	SDL_Renderer* renderer;
	
	//Paddle position
	Vector2 paddlePos;

	//Ball position
	Vector2 ballPos;

	//number of time since the beginning
	Uint32 ticksCount; 

	//The direction of the paddle
	// if the value is -1 => the paddle moves up, 1 => the paddle moves down, 0 the paddle does not move
	char paddleDir;

	//The velocity vector of the ball
	Vector2 ballVel;

	// Check if the game continue to run
	bool isRunning;
};
