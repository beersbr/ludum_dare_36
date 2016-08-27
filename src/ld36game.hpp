#ifndef _LD_LD36GAME_
#define _LD_LD36GAME_

#include "igame.hpp"

#include "keyboard.hpp"

#include "sprite.hpp"
#include "sprite.cpp"

#include "engine.hpp"


class LD36Game : public IGame {
public:
	virtual void End();
	virtual void Preload();
	virtual void Update();
	virtual void Render();

public:
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;

	shader_t shader;

};

#endif