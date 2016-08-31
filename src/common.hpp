#ifndef _LD_COMMON_ 
#define _LD_COMMON_

#include <SDL2/SDL.h>

#ifdef __APPLE__
#include <SDL2_Image/SDL_Image.h>
#include <Opengl/Opengl.h>
#include <Opengl/gl3.h>

#endif

#ifdef _WIN32
#include <SDL2/SDL_Image.h>
#include <GL/glew.h>
#include <gl/gl.h>
#include <math.h>
#include <algorithm>

#define fmin std::min
#define fmax std::max

#endif




#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/ext.hpp>

#include <iostream>
#include <cstdlib>
#include <cstddef>

#include <unordered_map>

#define INITIALIZATION_ERROR -5
#define ENTITY_NAME_SZ 64
#define RESOURCE_NAME_SZ 128

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

#define RANDOM(n) (rand()/(float)RAND_MAX * (n))
 
struct shader_t {
	GLuint id;
	char name[RESOURCE_NAME_SZ];
	bool enabled;
};

struct vertex_t {
	glm::vec3 position;
	glm::vec4 color;
	glm::vec2 uv;
};


struct rect_t {
	float x;
	float y;
	float width;
	float height;
};

//*******************************************************************************************************************
// FUNCTIONS
//*******************************************************************************************************************

bool const rectanglesCollide( const rect_t a, const rect_t b );

glm::vec3 SAT_AABBUncollisionVector(const rect_t subject, const rect_t passive );

char * const loadBinaryFile( const char * const filename );

shader_t loadShader( const char * const vertex_name, const char * const fragment_name, const char * const name );

#endif