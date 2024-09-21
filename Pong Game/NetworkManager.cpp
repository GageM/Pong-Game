#include "NetworkManager.h"

NetworkManager::NetworkManager(NetworkRole role_) : isInitialized(false), role(role_), isConnected(false), networkType(NetworkType::NONE)
{
}

NetworkManager::~NetworkManager()
{
	//delete[] TCPRecieveBuffer;
}

// Initialize & Shutdown Functions -------------------------

bool NetworkManager::InitUDPServer()
{
	if (isInitialized) return isInitialized;
	networkType = NetworkType::UDP;
	role = NetworkRole::server;

	// Initialize SDL Net
	if (SDLNet_Init() < 0)
	{
		SDL_Log("SDLNet initialization failed: %s", SDLNet_GetError());
		Shutdown();
		return false;
	}

	// Open socket
	UDPSocket = SDLNet_UDP_Open(8080);

	// Check if socket is open
	if (!UDPSocket)
	{
		std::cerr << "SDLNet_UDP_Open error: " << SDLNet_GetError() << std::endl;
		Shutdown();
		return false;
	}

	std::cout << "UDP server is running and listening on port 8080" << std::endl;

	// Allocate the recieve packet
	UDPReceivePacket = SDLNet_AllocPacket(1024);

	// Ensure the recieve packet was allocated
	if (!UDPReceivePacket) {
		SDL_Log("SDLNet_AllocPacket error: %s", SDLNet_GetError());
		Shutdown();
		return false;
	}

	// Wait for a client to connect to the server
	std::string data = std::string();
	while (data.size() == 0)
	{
		std::cout << "Waiting for client...\n";
		data = UDPRecieve();
	}

	std::cout << data << "\n";

	UDPSend("Server Connected");

	// Once we have received data from a client we are connected
	isConnected = true;


	isInitialized = true;
	return isInitialized;
}

bool NetworkManager::InitUDPClient()
{
	if (isInitialized) return isInitialized;
	networkType = NetworkType::UDP;
	role = NetworkRole::client;

	// Initialize SDL_net for UDP
	if (SDLNet_Init() < 0) {
		SDL_Log("SDLNet initialization failed: %s", SDLNet_GetError());
		Shutdown();
		return false;
	}

	UDPSocket = SDLNet_UDP_Open(0);

	if (!UDPSocket) {
		std::cerr << "SDLNet_UDP_Open error: " << SDLNet_GetError() << std::endl;
		Shutdown();
		return false;
	}

	UDPReceivePacket = SDLNet_AllocPacket(1024);

	if (!UDPReceivePacket) {
		SDL_Log("SDLNet_AllocPacket error: %s", SDLNet_GetError());
		Shutdown();
		return false;
	}

	// Connect to the server
	if (SDLNet_ResolveHost(&ip, "127.0.0.1", 8080) < 0) {
		std::cerr << "SDLNet_ResolveHost error: " << SDLNet_GetError() << std::endl;
		Shutdown();
		return false;
	}

	UDPReceivePacket->address = ip;

	std::cout << "UDP client is running and connected to the server on port 8080..." << std::endl;

	UDPSend("Client Connected");

	// Recieve server connected recipt
	std::string data = std::string();
	while (data.size() == 0)
	{
		std::cout << "Waiting for server to return connection...\n";
		data = UDPRecieve();
	}

	std::cout << data << "\n";

	isConnected = true;
	isInitialized = true;
	return isInitialized;
}

bool NetworkManager::InitTCPServer()
{
	if (isInitialized) return isInitialized;
	networkType = NetworkType::TCP;
	role = NetworkRole::server;

	// Initialize SDL Net
	if (SDLNet_Init() < 0)
	{
		SDL_Log("SDLNet initialization failed: %s", SDLNet_GetError());
		Shutdown();
		return false;
	}

	if (SDLNet_ResolveHost(&ip, NULL, 8081) < 0) {
		std::cerr << "SDLNet_ResolveHost error: " << SDLNet_GetError() << std::endl;
		Shutdown();
		return false;
	}

	// Open socket
	TCPServerSocket = SDLNet_TCP_Open(&ip);

	// Check if socket is open
	if (!TCPServerSocket)
	{
		std::cerr << "SDLNet_TCP_Open error: " << SDLNet_GetError() << std::endl;
		Shutdown();
		return false;
	}

	isInitialized = true;
	return isInitialized;
}

bool NetworkManager::InitTCPClient()
{
	if (isInitialized) return isInitialized;
	networkType = NetworkType::TCP;
	role = NetworkRole::client;

	// Initialize SDL Net
	if (SDLNet_Init() < 0)
	{
		SDL_Log("SDLNet initialization failed: %s", SDLNet_GetError());
		Shutdown();
		return false;
	}

	if (SDLNet_ResolveHost(&ip, "127.0.0.1", 8081) < 0) {
		std::cerr << "SDLNet_ResolveHost error: " << SDLNet_GetError() << std::endl;
		Shutdown();
		return false;
	}

	// Open socket
	TCPClientSocket = SDLNet_TCP_Open(&ip);

	// Check if socket is open
	if (!TCPClientSocket)
	{
		std::cerr << "SDLNet_TCP_Open error: " << SDLNet_GetError() << std::endl;
		Shutdown();
		return false;
	}

	TCPServerSocket = TCPClientSocket;

	isInitialized = true;
	return isInitialized;
}

void NetworkManager::Shutdown()
{
	switch (networkType)
	{
	case NetworkType::TCP:
		// Shutdown TCP networking
		if (TCPServerSocket) SDLNet_TCP_Close(TCPServerSocket);
		if (TCPClientSocket) SDLNet_TCP_Close(TCPClientSocket);
		break;
	case NetworkType::UDP:
		// Shutdown UDP networking
		if (UDPSocket)	SDLNet_UDP_Close(UDPSocket);
		if (UDPReceivePacket) SDLNet_FreePacket(UDPReceivePacket);
		break;
	default:
		break;
	}

	networkType = NetworkType::NONE;
	role = NetworkRole::NONE;

	// Close SDLNet
	SDLNet_Quit();

	if (UDPReceivePacket) UDPReceivePacket = nullptr;
	if (TCPServerSocket) TCPServerSocket = nullptr;
	if (TCPClientSocket) TCPClientSocket = nullptr;

	isConnected = false;
	isInitialized = false;
}

// Send & Recieve Functions --------------------------------

void NetworkManager::UDPSend(std::string data)
{
	if (networkType != NetworkType::UDP) return;

	// Create a UDP packet to send the position to the server
	UDPpacket* packet = SDLNet_AllocPacket(data.size() + 1);
	if (packet) {
		packet->address = UDPReceivePacket->address;
		packet->len = data.size();
		//memset(packet->data, '\0', packet->maxlen);
		memcpy(packet->data, data.c_str(), data.size() + 1);

		if (SDLNet_UDP_Send(UDPSocket, -1, packet) == 0) {
			std::cerr << "SDLNet_UDP_Send error: " << SDLNet_GetError() << std::endl;
		}

		SDLNet_FreePacket(packet);
	}

	SDL_Delay(16); // 16 Cap the sending rate to approximately 60 FPS
}

std::string NetworkManager::UDPRecieve()
{
	if (networkType != NetworkType::UDP) return std::string();

	std::string packetData;
	
	// Check for incoming UDP packets (data from the server)
	if (SDLNet_UDP_Recv(UDPSocket, UDPReceivePacket)) { 
		// Ensure the length does not exceed the length of UDPReceivePacket
		packetData = (std::string(reinterpret_cast<char*>(UDPReceivePacket->data), UDPReceivePacket->len));
	}

	return packetData;
}

void NetworkManager::TCPSend(std::string data)
{
	if (networkType != NetworkType::TCP) return;

	data.append("\0");

	//char sendBuffer[1024] = {};
	//strcpy_s(sendBuffer, sizeof(sendBuffer), data.c_str());
	//int len = strlen(sendBuffer);

	switch (role)
	{
	case NetworkRole::server:
		SDLNet_TCP_Send(TCPClientSocket, data.c_str(), data.size());
		break;
	case NetworkRole::client:
		SDLNet_TCP_Send(TCPServerSocket, data.c_str(), data.size());
		break;
	default:
		return;
		break;
	}	
}

std::string NetworkManager::TCPRecieve()
{
	if (networkType != NetworkType::TCP) return std::string();

	switch (role)
	{
	case NetworkRole::server:
		if (SDLNet_TCP_Recv(TCPClientSocket, TCPRecieveBuffer, sizeof(TCPRecieveBuffer)) > 0)
		{
			return std::string(TCPRecieveBuffer);
		}
		break;
	case NetworkRole::client:
		if (SDLNet_TCP_Recv(TCPServerSocket, TCPRecieveBuffer, sizeof(TCPRecieveBuffer)) > 0)
		{
			return std::string(TCPRecieveBuffer);
		}
		break;
	default:
		break;
	}
	return std::string();
}

// TCP Connect Functions -----------------------------------

bool NetworkManager::TCPConnectClient()
{
	if (role != NetworkRole::server) return false;

	// Accept a connection through the server socket
	TCPClientSocket = SDLNet_TCP_Accept(TCPServerSocket);
	if (TCPClientSocket)
	{
		printf("Client Connected\n");
		isConnected = true;
		return true;
	}
	return false;
}

bool NetworkManager::TCPDisconnectClient()
{
	if (role != NetworkRole::server) return false;

	printf("Client Disconnected\n");
	SDLNet_TCP_Close(TCPClientSocket);
	TCPClientSocket = nullptr;
	isConnected = false;
	return true;
}