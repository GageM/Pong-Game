#include <SDL.h>
#include "PongGame.h"

int main(int argc, char* argv[])
{
	NetworkRole role;
	char input = 'l';
	bool isInputValid = false;

	std::cout << "PONG!\n";

	// Ask whether to be the server or client
	while (!isInputValid)
	{
		std::cout << "Enter 'h' to host or 'c' to connect to a host : ";
		std::cin >> input;

		if (input == 'h' || input == 'c') isInputValid = true;
	}

	switch (input)
	{
	case 'h':
		role = NetworkRole::server;
		break;
	case 'c':
		role = NetworkRole::client;
		break;
	default:
		break;
	}

	// Start the game
	PongGame game{role};

	if (!game.Initialize()) return -1;

	std::thread sendThread(&PongGame::Send, std::ref(game));
	std::thread receiveThread(&PongGame::Receive, std::ref(game));

	game.RunGameLoop();

	sendThread.join();
	receiveThread.join();

	game.Shutdown();


	return 0;
}