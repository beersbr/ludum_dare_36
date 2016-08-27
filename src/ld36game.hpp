#ifndef _LD_LD36GAME_
#define _LD_LD36GAME_

#include "igame.hpp"
#include "igame.cpp"

class LD36Game : public IGame {
public:
	virtual void End();
	virtual void Preload();
	virtual void Update();
	virtual void Render();
};

#endif