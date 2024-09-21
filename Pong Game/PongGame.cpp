#include "PongGame.h"

#include <fstream>

#include "AESEncyptor.h"

PongGame::PongGame(NetworkRole role_) : isInitialized(false), isGameStarted(false), bgColor(Color::C64DarkGrey()), hostPaddle(nullptr), clientPaddle(nullptr), ball(nullptr),
	renderer(nullptr), window(nullptr), gameState(nullptr), role(role_)
{
	std::atomic_init(&doGameLoop, true);
}

PongGame::~PongGame()
{
}

void PongGame::Render(SDL_Renderer* renderer_) const
{
	//std::lock_guard<std::mutex> lg{ mtx };
	SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
	SDL_RenderClear(renderer);

	if (hostPaddle != nullptr) hostPaddle->Render(renderer);
	if (clientPaddle != nullptr) clientPaddle->Render(renderer);
	if (ball != nullptr) ball->Render(renderer);

	SDL_RenderPresent(renderer);
}

void PongGame::Update()
{
	ball->Update();
	ball->IsColliding(hostPaddle->GetPosition(), hostPaddle->GetDimensions());
	ball->IsColliding(clientPaddle->GetPosition(), clientPaddle->GetDimensions());
}

bool PongGame::Initialize()
{
	if (isInitialized) return isInitialized;

	// Initialize SDL Video for rendering
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
		return false;
	}

	// Create an SDL window and name it based on server role
	switch (role)
	{
	case NetworkRole::client:
		window = SDL_CreateWindow("SDL Pong Client", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
		break;
	case NetworkRole::server:
		window = SDL_CreateWindow("SDL Pong Server", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
		break;
	default:
		Shutdown();
		return false; // Server role somehow got messed up
		break;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	// Ensure the window exists
	if (!window || !renderer) {
		std::cerr << "Failed to create window or renderer: " << SDL_GetError() << std::endl;
		Shutdown();
		return false;
	}

	networkManager = std::make_shared<NetworkManager>(role);

	switch (role)
	{
	case NetworkRole::server:
		if (!networkManager->InitUDPServer()) return false;
		break;
	case NetworkRole::client:
		if (!networkManager->InitUDPClient()) return false;
		break;
	default:
		break;
	}

	// Initialize the game state
	gameState = std::make_shared<GameState>("gameState", 5);

	// Initialize the sprites
	hostPaddle = std::make_shared<Paddle>("hostPaddle", NetworkRole::server, width, height);
	clientPaddle = std::make_shared<Paddle>("clientPaddle", NetworkRole::client, width, height);
	ball = std::make_shared<Ball>("ball", width, height);

	isInitialized = true;
	return isInitialized;
}

void PongGame::Shutdown()
{
	if (isInitialized)
	{
		switch (role)
		{
		case NetworkRole::server:
			networkManager->UDPSend("Server Disconnected");
			break;
		case NetworkRole::client:
			networkManager->UDPSend("Client Disconnected");
			break;
		default:
			break;
		}
	}

	networkManager->Shutdown();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	if (renderer)
	{
		renderer = nullptr;
	}

	if (window)
	{
		window = nullptr;
	}

	hostPaddle = nullptr;
	clientPaddle = nullptr;
	ball = nullptr;

	isInitialized = false;
}

void PongGame::Send()
{
	while (doGameLoop.load())
	{
		//std::lock_guard<std::mutex> lg{ mtx };
	
		switch (role)
		{
		case NetworkRole::client:
			networkManager->UDPSend(clientPaddle->Serialize());
			break;
		case NetworkRole::server:
			networkManager->UDPSend(hostPaddle->Serialize());
			networkManager->UDPSend(ball->Serialize());
			networkManager->UDPSend(gameState->Serialize());
			break;
		default:
			break;
		}

		//SDL_Delay(16); // Cap the sending rate to approximately 60 FPS
	}
}

void PongGame::Receive()
{
	while (doGameLoop.load())
	{
		DeSerialize();	
		//printf("End Recieve\n");
	}
}

void PongGame::DeSerialize()
{
	// Recieve data
	rapidjson::Document doc;
	std::string data = networkManager->UDPRecieve();

	if (data.size() == 0) return;

	doc.Parse(data.c_str());

	if (!doc.IsObject())
	{
		printf("%s\n", data.c_str());
		return;
	}

	// Get the name of the object that was sent
	const rapidjson::Value& nameValue = doc["name"];

	if (!nameValue.IsString()) return;

	std::string name = nameValue.GetString();

	std::lock_guard<std::mutex>{ mtx };

	// Check which object to update
	if (name == "ball")
	{
		ball->DeSerialize(doc);
		//ball->Print();
		return;
	}
	else if (name == "hostPaddle")
	{
		hostPaddle->DeSerialize(doc);
		//hostPaddle->Print();
		return;
	}
	else if (name == "clientPaddle")
	{
		clientPaddle->DeSerialize(doc);
		//clientPaddle->Print();
		return;
	}
	else if (name == "gameState")
	{
		gameState->DeSerialize(doc);
		return;
	}
}

void PongGame::RunGameLoop()
{
	while (doGameLoop.load())
	{
		while (SDL_PollEvent(&event))
		{
			// Check if SDL has quit
			if (event.type == SDL_QUIT)
			{

			}

			// Check for input from the player
			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				// Start game button
				{
				case SDLK_RETURN:
					if (!isGameStarted && networkManager->isConnected) isGameStarted = true;
					SDL_Delay(500);
					break;
				}

				// Exit game button		
				{
				case SDLK_q:
					// Provide enough time to send cleared data to client
					SDL_Delay(100);
					doGameLoop.store(false);
					break;
				}

				// Player movement
				{
				case SDLK_UP:
					switch (role)
					{						
					case NetworkRole::client:
						clientPaddle->SetPosition(clientPaddle->GetPosition() - Vector2::up() * clientPaddle->GetMoveSpeed());
						break;
					case NetworkRole::server:
						hostPaddle->SetPosition(hostPaddle->GetPosition() - Vector2::up() * hostPaddle->GetMoveSpeed());
						break;
					default:
						break;
					}
					break;
				case SDLK_DOWN:
					switch (role)
					{
					case NetworkRole::client:
						clientPaddle->SetPosition(clientPaddle->GetPosition() + Vector2::up() * clientPaddle->GetMoveSpeed());
						clientPaddle->Update();
						break;
					case NetworkRole::server:
						hostPaddle->SetPosition(hostPaddle->GetPosition() + Vector2::up() * hostPaddle->GetMoveSpeed());
						hostPaddle->Update();
						break;
					default:
						break;
					}
					break;
				}
				}
			}
		}

		if (role == NetworkRole::server && isGameStarted)
		{
			Update();
			int playerScored = ball->IsOOB();
			if (playerScored) ScorePoint(playerScored);
		}
		else if (role == NetworkRole::client)
		{
			Update();
			int playerScored = ball->IsOOB();
			if (playerScored) ScorePoint(playerScored);
		}

		Render(renderer);

		// Check for game over
		if (gameState->hostScore >= gameState->scoreToWin)
		{
			printf("Host Wins Game\n");
			EndGame();
		}
		else if(gameState->clientScore >= gameState->scoreToWin)
		{
			printf("Client Wins Game\n");
			EndGame();
		}
	}
}

void PongGame::ScorePoint(const int player)
{
	switch (player)
	{
	case 1:
		gameState->hostScore++;
		break;
	case 2:
		gameState->clientScore++;
		break;
	default:
		break;
	}

	ball->Reset();

	//isGameStarted = false;
}

void PongGame::EndGame()
{
	doGameLoop = false;

	gameState->Print();

	char input = 'l';
	bool isInputValid = false;

	switch (role)
	{
	case NetworkRole::server:
		// Ask whether to quit or send data to the server
		while (!isInputValid)
		{
			std::cout << "Enter 'q' to quit or 's' to send score to scorekeeping server: ";
			std::cin >> input;

			if (input == 'q' || input == 's') isInputValid = true;
		}
		break;
	case NetworkRole::client:
		// Ask whether to quit 
		while (!isInputValid)
		{
			std::cout << "Enter 'q' to quit: ";
			std::cin >> input;

			if (input == 'q') isInputValid = true;
		}
		break;
	default:
		break;
	}



	if (role != NetworkRole::server) return;

	networkManager->UDPSend(gameState->Serialize());

	// Close server
	networkManager->Shutdown();

	// Serialize game state
	std::string unencryptedGameState = gameState->Serialize();

	// Encrypt game state
	AESEncyptor encryptor;

	unsigned char* AESKey = encryptor.GenerateAESKey("password");
	std::string encryptedGameState = encryptor.Encrypt(unencryptedGameState, AESKey);

	// Save the game state to a local file
	std::ofstream file;
	file.open("PongScore.txt");

	if (!file.is_open())
	{
		std::cerr << "Error: Could not open file for writing\n";
	}

	file << encryptedGameState;

	file.close();

	if (input == 'q') return;

	// Connect to scorekeeping server
	networkManager->InitTCPClient();

	// Send game state to scorekeeping server
	networkManager->TCPSend(encryptedGameState);
}
