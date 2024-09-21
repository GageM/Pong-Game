#pragma once

#include <SDL.h>
#include <iostream>
#include <cstring>
#include <sstream>
#include <thread>
#include <memory>
#include <atomic>
#include <mutex>

#include "NetworkManager.h"
#include "Ball.h"
#include "Paddle.h"
#include "GameState.h"

template<typename T>
using Ref = std::shared_ptr<T>;


class PongGame : Renderable
{
public:
	PongGame(NetworkRole role_);
	~PongGame();

	void Render(SDL_Renderer* renderer_) const override;
	void Update() override;

	bool Initialize();
	void Shutdown();

	void Send();
	void Receive();
	void DeSerialize();

	void RunGameLoop();

private:
	Ref<Paddle> hostPaddle;
	Ref<Paddle> clientPaddle;
	Ref<Ball> ball;
	Ref<GameState> gameState;

	Ref<NetworkManager> networkManager;
	NetworkRole role;

	std::atomic<bool> doGameLoop;
	bool isInitialized;
	bool isGameStarted;
	bool isClientConnected;

	Color bgColor;

	// SDL Properties
	const int width = 1280;
	const int height = 720;

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;

	void ScorePoint(const int player);
	void EndGame();

	mutable std::mutex mtx;
};

