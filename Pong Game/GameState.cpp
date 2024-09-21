#include "GameState.h"

GameState::GameState(const char* name_, int scoreToWin_) : SerializableObject(name_), hostScore(0), clientScore(0), scoreToWin(scoreToWin_)
{
}

std::string GameState::Serialize()
{
    rapidjson::StringBuffer s;
    rapidjson::Writer<rapidjson::StringBuffer> writer(s);

    writer.StartObject();
    writer.Key("name");
    writer.String(name);
    writer.Key("hostScore");
    writer.Double(hostScore);
    writer.Key("clientScore");
    writer.Double(clientScore);
    writer.Key("scoreToWin");
    writer.Double(scoreToWin);
    writer.EndObject();

    return s.GetString();
}

void GameState::DeSerialize(const rapidjson::Document& doc)
{
    const rapidjson::Value& hostScoreValue = doc["hostScore"];
    const rapidjson::Value& clientScoreValue = doc["clientScore"];
    const rapidjson::Value& scoreToWinValue = doc["scoreToWin"];

    if (!hostScoreValue.IsInt() || !clientScoreValue.IsInt() || !scoreToWinValue.IsInt()) return;

    hostScore = hostScoreValue.GetInt();
    clientScore = clientScoreValue.GetInt();
    scoreToWin = scoreToWinValue.GetInt();
}

void GameState::Print(const char* msg)
{
    if (msg != nullptr) std::cout << msg << "\n";
    std::cout << "Host Score: " << hostScore << "\n";
    std::cout << "Client Score: " << clientScore << "\n";
    std::cout << "Score To Win: " << scoreToWin << "\n";

}
