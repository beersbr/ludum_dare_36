#ifndef _LD_SPRITE_
#define _LD_SPRITE_

#include <SDL2/SDL.h>
#include <SDL2_Image/SDL_Image.h>
#include <Opengl/Opengl.h>
#include <Opengl/gl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/ext.hpp>

#include "timer.hpp"

#include "common.hpp"

// struct vertex_t {
// 	glm::vec3 position;
// 	glm::vec4 color;
// 	glm::vec2 uv;
// };

//*******************************************************************************************************************
// TEXTURE
//*******************************************************************************************************************


class Texture {
public:
	Texture();
	~Texture();

	void Load( const char * const filename, const int frame_columns, const int frame_rows );
	void Use() const;

public:
	static long ids;
	static GLuint currentRsId;

	SDL_Surface* pixels;
	glm::vec2    size;
	GLuint       rsId;
	long         id;
	bool         loaded;
	int          currentTextureUnit;
	std::string  name;
};


//*******************************************************************************************************************
// MESH
//*******************************************************************************************************************


class Mesh {
public: 
	static Mesh * const GetWithId(const int id);

	Mesh();
	~Mesh();

	void Use() const; 
	void Initialize( const vertex_t * const vertices, const int vertices_sz );

public:	
	long id;
	GLuint vaoId;
	GLuint vboId;

	vertex_t *vertices;
	int verticesSz;

private:
	static Mesh** pool;
	static int    poolSize;
	static int    availableSize;
	static long   ids;
	static long   currentId;

	bool loaded;
};


//*******************************************************************************************************************
// SPRITE
//*******************************************************************************************************************


class Sprite {
public:
	Sprite();

	void Render() const;

	void StepFrame();
	void SetFrame(const int frame);

public:
	static long ids;

	Timer frameTimer;
	int frameMilliseconds;

	long id;
	int  direction;

	float width;
	float height;

	int frameCols;
	int frameRows;
	int currentFrame;
	int totalFrames;

	Mesh*    mesh;
	Texture* texture;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 uv;

	std::string name;
};

#endif