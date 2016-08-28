#ifndef _LD_GAME_OBJECT_
#define _LD_GAME_OBJECT_

#include "sprite.hpp"
#include "timer.hpp"

enum object_type_t { 
	OBJECT_TYPE_NONE = 0,
	OBJECT_TYPE_SOLID,
	OBJECT_TYPE_COUNT
};

enum object_name_t {
	OBJECT_NAME_NONE = 0,
	OBJECT_NAME_BLOCK,
	OBJECT_NAME_ENEMY,
	OBJECT_NAME_DASH,
};

class GameObject {
public:
	GameObject();
	virtual ~GameObject();

	virtual void Update(const long elapsedMilliseconds);
	virtual void Render(glm::mat4 *p, glm::mat4 *v);

	virtual void CollisionWith(GameObject *collisionTarget);
	virtual rect_t const GetRect() const;

public:
	long id;
	std::string name;

	bool visible;
	bool alive;

	Sprite sprite;
	object_type_t objectType;
	object_name_t objectName;
	int lifeTimeLength;
	Timer lifeTimeTimer;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::vec3 velocity;
	glm::vec3 acceleration;

};


#endif