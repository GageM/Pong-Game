#pragma once

#include <SDL.h>
#include <SDL_net.h>
#include <iostream>
#include <cstring>
#include <sstream>
#include <thread>
#include <memory>
#include <atomic>

#include "NetworkRole.h"

class NetworkManager
{
public:
	NetworkManager(NetworkRole role_);
	~NetworkManager();

	bool isConnected;

	bool InitUDPServer();
	bool InitUDPClient();

	bool InitTCPServer();
	bool InitTCPClient();

	void Shutdown();

	void UDPSend(std::string data);
	std::string UDPRecieve();

	void TCPSend(std::string data);
	std::string TCPRecieve();

	bool TCPConnectClient();
	bool TCPDisconnectClient();

private:
	bool isInitialized;
	NetworkRole role;
	NetworkType networkType;

	// UDP Networking Variables
	UDPsocket UDPSocket;
	UDPpacket* UDPReceivePacket;

	TCPsocket TCPServerSocket;
	TCPsocket TCPClientSocket;
	char TCPRecieveBuffer[1024];

	IPaddress ip;
};

