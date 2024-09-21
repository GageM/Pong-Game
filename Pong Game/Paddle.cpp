#include "Paddle.h"

Paddle::Paddle(const char* name_, NetworkRole role, int screenWidth, int screenHeight) : SerializableObject(name_), color(Color::C64Orange()), dimensions(40, 200), moveSpeed(10.0f), screenSize(screenWidth, screenHeight)
{
    switch (role)
    {
    case NetworkRole::client:
        position = Vector2(screenWidth - dimensions.x, screenHeight / 2 - dimensions.y / 2);
        break;
    case NetworkRole::server:
        position = Vector2(0, screenHeight / 2 - dimensions.y / 2);
        break;
    default:
        break;
    }
}

std::string Paddle::Serialize()
{
    rapidjson::StringBuffer s;
    rapidjson::Writer<rapidjson::StringBuffer> writer(s);

    writer.StartObject();
    writer.Key("name");
    writer.String(name);
    writer.Key("xPosition");
    writer.Double(position.x);
    writer.Key("yPosition");
    writer.Double(position.y);
    writer.EndObject();

    return s.GetString();
}

void Paddle::DeSerialize(const rapidjson::Document& doc)
{
    const rapidjson::Value& xPosValue = doc["xPosition"];
    const rapidjson::Value& yPosValue = doc["yPosition"];

    if (!xPosValue.IsDouble() || !yPosValue.IsDouble()) return;

    position.x = xPosValue.GetDouble();
    position.y = yPosValue.GetDouble();
}

void Paddle::Print(const char* msg)
{
    if (msg != nullptr) std::cout << msg << "\n";
    std::cout << name << ":\n";
    std::cout << "Position: " << position.x << ", " << position.y << "\n";
}

void Paddle::Render(SDL_Renderer* renderer) const
{
    // Draw the Paddle
    SDL_Rect squareRect = { position.x, position.y, dimensions.x, dimensions.y };
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &squareRect);
}

void Paddle::Update()
{
    if (position.y < 0) position.y = 0;
    if (position.y + dimensions.y > screenSize.y) position.y = screenSize.y - dimensions.y;
}

bool Paddle::IsColliding(const Vector2& otherPos, const Vector2 otherDimensions)
{
    return false;
}
