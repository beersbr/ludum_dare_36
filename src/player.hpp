#ifndef _LD_PLAYER_
#define _LD_PLAYER_

#include "game_object.hpp"
#include "keyboard.hpp"
#include "sprite.hpp"

#include "engine.hpp"

class Player : public GameObject {
public:
	Player();
	virtual ~Player();

	virtual void Update(const long elapsedMilliseconds);
	virtual void Render(glm::mat4 *p, glm::mat4 *v);
	virtual void CollisionWith(GameObject *collisionTarget);
};

#endif