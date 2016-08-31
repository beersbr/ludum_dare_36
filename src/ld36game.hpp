#ifndef _LD_LD36GAME_
#define _LD_LD36GAME_

#ifdef __APPLE__
#include <unistd.h>
#endif 

#include "igame.hpp"

#include "keyboard.hpp"

#include "sprite.hpp"
// #include "sprite.cpp"

#include "engine.hpp"
#include "player.hpp"
#include "eye_guy_game_object.hpp"

class LD36Game : public IGame {
public:
	virtual void End();
	virtual void Preload();
	virtual void Update(const long elapsedMilliseconds);
	virtual void EntityUpdate(const long elapsedMilliseconds);
	virtual void Render();

public:

	void SpawnMonster() const;

	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;

	Timer monsterSpawnTimer;
	int monsterSpawnTimout;

	shader_t shader;

};

#endif