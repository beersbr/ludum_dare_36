#ifndef _LD_ENGINE_
#define _LD_ENGINE_

#include <SDL2/SDL.h>

#ifdef __APPLE__
#include <SDL2_Image/SDL_Image.h>
#include <Opengl/Opengl.h>
#include <Opengl/gl3.h>
#include <unistd.h>

#endif


#ifdef _WIN32
#include <SDL2/SDL_Image.h>
#include <SDL2/SDL_Image.h>
#include <GL/glew.h>
#include <gl/gl.h>
#include <math.h>
#endif


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/ext.hpp>

#include <iostream>
#include <cstdlib>
#include <cstddef>


#include <unordered_map>
#include <list>

#ifdef _WIN32
#include <GL/glew.h>
#endif

#include "igame.hpp"
#include "timer.hpp"
#include "game_object.hpp"


class Camera { 
public:
	Camera();
	~Camera();

	void FollowTarget(); 
	void IgnoreTarget(); 
	void SetLookat(const glm::vec3 lookat);
	void SetTarget(GameObject * const object);
	void Update();

public:
	float drag;
	GameObject *target;
	bool followTarget;
	glm::vec3 lookat;
	glm::mat4 view;

};



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

	Camera camera;

	GameObject *player;


};

extern LD36Engine* Engine;

#endif