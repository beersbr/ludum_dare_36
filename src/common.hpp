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

enum objectType_t {
	NONE = 0,
	PLAYER,
	OBJECT_TYPE_COUNT,
};
 
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


struct spriteSheet_t {
	long id;
	SDL_Surface *image;
	glm::vec2 size;
	GLuint ident;
	bool loaded;
	char name[RESOURCE_NAME_SZ];
};


struct mesh_t { 
	GLuint renderMode;
	vertex_t *vertices;
	int verticesSz;
};


struct renderable_t {
	long id;
	bool valid;
	GLuint vao;
	GLuint vbo;

	mesh_t * mesh;
};


struct sprite_t {
	spriteSheet_t *spriteSheet;
	glm::vec2 frameSize;
	glm::vec3 frameScale;
	glm::vec2 frameOrigin;

	int framesTotal;
	int currentFrame;

	glm::mat4 uvmodel;

	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;

	renderable_t * renderable;
	shader_t * shader;
};

void (*updateFn)(float time);

struct gameObject_t {
	long id;
	char name[ENTITY_NAME_SZ];
	bool visible;
	bool alive;
	objectType_t objectType;

	glm::vec3 position;
	glm::vec3 size; 
	glm::vec3 rotation;

	glm::vec2 velocity;
	glm::vec2 acceleration;

	sprite_t sprite;
};

struct gameObjectManager_t {
	void * pool;
	gameObject_t * gameObjects;
	long poolSize;
	long maxObjectsCount;
	long objectCount;
};


//*******************************************************************************************************************
// FUNCTIONS
//*******************************************************************************************************************

char * const loadBinaryFile( const char * const filename );

shader_t loadShader( const char * const vertex_name, const char * const fragment_name, const char * const name );

renderable_t * createRenderable( mesh_t * const mesh);

void bindRenderable(renderable_t * const renderable);

spriteSheet_t loadSpriteSheet( const char * const filename );

sprite_t createSprite( const spriteSheet_t * const sprite_sheet, const shader_t * const shader, glm::vec2 origin, glm::vec2 frame_size, int frame_count );

void renderSprite( const glm::mat4 * const projectionMatrix, const glm::mat4 * const viewMatrix, const sprite_t * const sprite );

void setSpriteFrame( sprite_t * const sprite, const int frame );

void stepSpriteFrame(sprite_t * const sprite );

mesh_t * getTileMesh();

gameObjectManager_t createGameObjectManager(const long max_objects);

#endif