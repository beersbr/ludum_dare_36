#ifndef _LD_GAME_OBJECT_
#define _LD_GAME_OBJECT_

#include "sprite.hpp"
#include "timer.hpp"

enum object_type_t { 
	OBJECT_TYPE_NONE = 0,
	OBJECT_TYPE_COUNT
};

class GameObject {
public:
	long id;
	std::string name;

	bool visible;
	bool alive;

	object_type_t objectType;
	int lifeTimeLength;
	Timer lifeTimer;

};


#endif