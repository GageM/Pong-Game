#include <SDL.h>
#include <fstream>

#include "NetworkManager.h"
#include "AESEncyptor.h"
#include "GameState.h"

int RunPongScoreServer()
{
	char input = 'l';
	bool isInputValid = false;

	std::cout << "Pong Scorekeeping Server\n";

	// Ask whether to host the server or read a file
	while (!isInputValid)
	{
		std::cout << "Enter 's' to open the server or 'r' to read the score file: ";
		std::cin >> input;

		if (input == 's' || input == 'r') isInputValid = true;
	}

	// Host TCP server
	if (input == 's')
	{
		// Open server for hosting

		NetworkManager networkManager(NetworkRole::server);

		if (!networkManager.InitTCPServer())
		{
			return -1;
		}

		// Connect a client

		while (!networkManager.isConnected)
		{
			networkManager.TCPConnectClient();
			printf("Waiting for client...\n");
		}

		// Recieve results

		std::string results = std::string();

		while (results.size() == 0)
		{
			printf("Waiting for results...\n");
			results = networkManager.TCPRecieve();
		}

		AESEncyptor encryptor;
		unsigned char* key = encryptor.GenerateAESKey("password");

		std::string decryptedResults = encryptor.Decrypt(results, key);

		std::cout << decryptedResults << "\n";

		// Save results to score file

		std::ofstream file;
		file.open("PongScore.txt");

		if (!file.is_open())
		{
			std::cerr << "Error: Could not open file for writing\n";
			return -1;
		}

		file << results;

		// Shutdown

		file.close();

		networkManager.TCPDisconnectClient();

		networkManager.Shutdown();

		isInputValid = false;
		while (!isInputValid)
		{
			std::cout << "Enter 'q' to quit: ";
			std::cin >> input;

			if (input == 'q') isInputValid = true;
		}
	}

	// Read the score file
	else if (input == 'r')
	{
		// Open the score file
		std::ifstream file;
		file.open("PongScore.txt");

		if (!file.is_open())
		{
			std::cerr << "Error: Could not open file for reading\n";
			return -1;
		}

		// Decrypt the score
		AESEncyptor encryptor;
		unsigned char* key = encryptor.GenerateAESKey("password");

		std::string encryptedScore;

		file >> encryptedScore;

		std::string decryptedScore = encryptor.Decrypt(encryptedScore, key);

		// Print the score

		GameState score("gameState", 5);

		rapidjson::Document doc;
		doc.Parse(decryptedScore.c_str());

		if (doc.IsObject())
		{
			score.DeSerialize(doc);
			score.Print();
		}

		file.close();

		isInputValid = false;
		while (!isInputValid)
		{
			std::cout << "Enter 'q' to quit: ";
			std::cin >> input;

			if (input == 'q') isInputValid = true;
		}
	}

	return 0;
}

int TestUDPNetwork()
{
	char input = 'l';
	bool isInputValid = false;

	std::cout << "UDP Network Test\n";

	// Ask whether to be host or client
	while (!isInputValid)
	{
		std::cout << "Enter 's' to open the server or 'c' to connect to a server: ";
		std::cin >> input;

		if (input == 's' || input == 'c') isInputValid = true;
	}

	// Run test server
	if (input == 's')
	{
		NetworkManager nm(NetworkRole::server);
		nm.InitUDPServer();

		auto recieveLambda = [](NetworkManager& nm) {while (1) { std::cout << nm.UDPRecieve() << "\n"; }};
		auto sendLambda = [](NetworkManager& nm) {while (1) { nm.UDPSend("Test Message"); SDL_Delay(16); }};

		//std::thread recieveThread(recieveLambda, std::ref(nm));
		//std::thread sendThread(sendLambda, std::ref(nm));

		//std::string data = std::string();

		//while (data.size() == 0)
		//{
		//	std::cout << "Waiting for data...\n";
		//	data = nm.UDPRecieve();
		//}
		//
		//std::cout << data << "\n";

		//recieveThread.join();
		//sendThread.join();

		isInputValid = false;
		while (!isInputValid)
		{
			std::cout << "Enter 'q' to quit: ";
			std::cin >> input;

			if (input == 'q') isInputValid = true;
		}
	}

	// Run test client
	else if (input == 'c')
	{
		NetworkManager nm(NetworkRole::client);
		nm.InitUDPClient();

		auto recieveLambda = [](NetworkManager& nm) {while (1) { std::cout << nm.UDPRecieve() << "\n"; }};
		auto sendLambda = [](NetworkManager& nm) {while (1) { nm.UDPSend("Test Message"); SDL_Delay(16); }};

		//std::thread recieveThread(recieveLambda, std::ref(nm));
		//std::thread sendThread(sendLambda, std::ref(nm));

		//recieveThread.join();
		//sendThread.join();

		//nm.UDPSend("Hello");
	}

	isInputValid = false;
	while (!isInputValid)
	{
		std::cout << "Enter 'q' to quit: ";
		std::cin >> input;

		if (input == 'q') isInputValid = true;
	}

	return 0;
}

int TestEncryptor()
{
	AESEncyptor encryptor{};

	std::string testString = "{xPos, 7.99999833, yPos, 19.000003, pizza, words, z, !@#$%^&*()}";
	std::string password = "password";

	std::cout << testString << "\n";

	unsigned char* key = encryptor.GenerateAESKey(password);

	std::string encryptedData = encryptor.Encrypt(testString, key);
	std::cout << encryptedData << "\n";

	std::string decryptedData = encryptor.Decrypt(encryptedData, key);
	std::cout << decryptedData << "\n";

	return 0;
}

int main(int argc, char* argv[])
{
	return RunPongScoreServer();
	//return TestEncryptor();
}