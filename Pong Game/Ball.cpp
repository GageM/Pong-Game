#include "Ball.h"

Ball::Ball(const char* name_, int screenWidth, int screenHeight) : SerializableObject(name_), color(Color::C64LightGreen()), position(screenWidth / 2, screenHeight / 2), radius(25.0f),
    velocity(0.04f, 0.02f), screenSize(screenWidth, screenHeight)
{

}

std::string Ball::Serialize()
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
    writer.Key("xVelocity");
    writer.Double(velocity.x);
    writer.Key("yVelocity");
    writer.Double(velocity.y);
    writer.EndObject();

    return s.GetString();
}

void Ball::DeSerialize(const rapidjson::Document& doc)
{
    const rapidjson::Value& xPosValue = doc["xPosition"];
    const rapidjson::Value& yPosValue = doc["yPosition"];

    const rapidjson::Value& xVelValue = doc["xVelocity"];
    const rapidjson::Value& yVelValue = doc["yVelocity"];

    if (!xPosValue.IsDouble() || !yPosValue.IsDouble()) return;
    if (!xVelValue.IsDouble() || !yVelValue.IsDouble()) return;

    position.x = xPosValue.GetDouble();
    position.y = yPosValue.GetDouble();

    velocity.x = xVelValue.GetDouble();
    velocity.y = yVelValue.GetDouble();
}

void Ball::Print(const char* msg)
{
    if (msg != nullptr) std::cout << msg << "\n";
    std::cout << name << ":\n";
    std::cout << "Position: " << position.x << ", " << position.y << "\n";
    std::cout << "Velocity: " << velocity.x << ", " << velocity.y << "\n";
}

void Ball::Render(SDL_Renderer* renderer) const
{
    // Draw the Ball
    SDL_Rect squareRect = { position.x, position.y, radius, radius };
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &squareRect);
}

void Ball::Update()
{
    position += velocity;

    // Bounce the ball when hitting the top or bottom of the screen
    if (position.y < 0 )
    {
        velocity.y *= -1;
        position.y = 0;
    }

    if (position.y + radius > screenSize.y)
    {
        velocity.y *= -1;
        position.y = screenSize.y - radius;
    }
}

bool Ball::IsColliding(const Vector2& otherPos, const Vector2 otherDimensions)
{
    if (position.y + radius > otherPos.y && position.y < otherPos.y + otherDimensions.y)
    {
        if (position.x + radius > otherPos.x && position.x < otherPos.x + otherDimensions.x)
        {
            velocity.x *= -1;
        }
    }

    return false;
}

int Ball::IsOOB()
{
    if (position.x < 0)
    {
        return 2;
    }

    else if (position.x + radius > screenSize.x)
    {
        return 1;
    }    
    
    return 0;
}

void Ball::Reset()
{
    position = Vector2(screenSize.x / 2, screenSize.y / 2);
}
