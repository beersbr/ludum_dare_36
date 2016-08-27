#ifndef _LD_COMMON_ 
#define _LD_COMMON_

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

#define INITIALIZATION_ERROR -5
#define ENTITY_NAME_SZ 64
#define RESOURCE_NAME_SZ 128

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800
 
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

//*******************************************************************************************************************
// FUNCTIONS
//*******************************************************************************************************************

char * const loadBinaryFile( const char * const filename );

shader_t loadShader( const char * const vertex_name, const char * const fragment_name, const char * const name );

#endif