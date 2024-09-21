#pragma once

#include <SDL.h>

class Renderable
{
	virtual void Render(SDL_Renderer* renderer) const = 0;
	virtual void Update() = 0;
};
