#ifndef _LD_IGAME_
#define _LD_IGAME_

class IGame {
public:
	virtual void End()                                  = 0;
	virtual void Preload()                              = 0;
	virtual void Update(const long elapsedMilliseconds) = 0;
	virtual void Render()                               = 0;
};

#endif