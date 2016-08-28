#ifndef _LD_EYE_GUY_
#define _LD_EYE_GUY_

#include "game_object.hpp"
#include "sprite.hpp"
#include "engine.hpp"

class EyeGuyGameObject : public GameObject {
public:
	EyeGuyGameObject();
	virtual ~EyeGuyGameObject();

	virtual void Update(const long elapsedMilliseconds);
	virtual void Render(glm::mat4 *p, glm::mat4 *v);
	virtual void CollisionWith(GameObject *collisionTarget);
};


#endif