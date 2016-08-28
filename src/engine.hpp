#ifndef _LD_ENGINE_
#define _LD_ENGINE_

#include <SDL2/SDL.h>
#include <SDL2_Image/SDL_Image.h>
// #include <SDL2/SDL_opengl.h>
#include <Opengl/Opengl.h>
#include <Opengl/gl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/ext.hpp>

#include <iostream>
#include <cstdlib>
#include <cstddef>
#include <unistd.h>

#include <unordered_map>
#include <list>

#ifdef _WIN32
#include <GL/glew.h>
#endif

#include "igame.hpp"
#include "timer.hpp"
#include "game_object.hpp"

class LD36Engine {
public:
	LD36Engine();
	virtual ~LD36Engine();

	void Initialize(const int window_width, const int window_height, const char * const title);
	bool IsValid() const;
	bool IsRunning() const;

	void Update();
	void Shutdown();

public:
	int           windowWidth;
	int           windowHeight;
	SDL_Window *  window;
	SDL_GLContext context;

	bool valid;
	bool running;
	bool paused;

	Timer coreTimer;
	long  coreFrameCount;
	long  coreFrameRate;

	Timer realTimer;
	long  realFrameCount;
	long  realFrameRate;

	std::list<GameObject*> objects;

	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;

	IGame *game;
};

extern LD36Engine* Engine;

#endif