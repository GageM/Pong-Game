#pragma once

#include <iostream>

// Note AESEncryptor would have used OpenSSL Encryption methods but I couldn't get vcpkg to work right

class AESEncyptor
{
public:
	unsigned char* GenerateAESKey(std::string password);
	std::string Encrypt(std::string input, const unsigned char* key);
	std::string Decrypt(std::string input, const unsigned char* key);
};

