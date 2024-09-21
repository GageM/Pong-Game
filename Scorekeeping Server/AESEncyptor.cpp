#include "AESEncyptor.h"

#include <unordered_map>

unsigned char* AESEncyptor::GenerateAESKey(std::string password)
{
	size_t hash = std::hash<std::string>()(password);
	// Seperate hash into 8 unsigned chars to create key
	unsigned char key[sizeof(size_t)] = {};
	memcpy(key, &hash, sizeof(size_t));
	return key;
}

std::string AESEncyptor::Encrypt(std::string input, const unsigned char* key)
{
	std::string encryptedData;
	encryptedData = input;

	for (size_t i = 0; i < input.size(); i++)
	{
		encryptedData[i] += 1;// key[i % strlen((char*)key)];
	}

	return encryptedData;
}

std::string AESEncyptor::Decrypt(std::string input, const unsigned char* key)
{
	std::string unencryptedData;
	unencryptedData = input;

	for (size_t i = 0; i < input.size(); i++)
	{
		unencryptedData[i] -= 1;// key[i % strlen((char*)key)];
	}

	return unencryptedData;
}
