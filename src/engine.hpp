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

#include <unordered_map>

#ifdef _WIN32
#include <GL/glew.h>
#endif

#include "timer.hpp"
#include "timer.cpp"

class LD36Engine {
public:
	LD36Engine();
	virtual ~LD36Engine();

	void Initialize(const int window_width, const int window_height, const char * const title);
	bool IsValid() const;


	void Update();


public:
	int windowWidth;
	int windowHeight;
	SDL_Window *window;
	SDL_GLContext context;

	bool valid;
	bool running;
	bool paused;
};

extern LD36Engine* Engine;

#endif