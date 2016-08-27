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

#include <unordered_map>

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

	static Texture* CreateTexture( const std::string name, const char * const filename);

	void Load( const char * const filename );
	void Use() const;

public:
	static long ids;
	static GLuint currentRsId;
	static std::unordered_map<std::string, Texture*> pool;

	SDL_Surface* pixels;
	long 		 width;
	long 		 height;
	GLuint       rsId;
	long         id;
	bool         loaded;
	int          currentTextureUnit;
};


//*******************************************************************************************************************
// MESH
//*******************************************************************************************************************


class Mesh {
public: 
	static Mesh * CreateMesh(const std::string name, const vertex_t * const vertices, const int vertices_sz );

	Mesh();
	~Mesh();

	void Use() const; 
	void Initialize( const vertex_t * const vertices, const int vertices_sz );
	void SetRenderMode(GLuint mode);

public:	
	long id;
	GLuint vaoId;
	GLuint vboId;
	GLuint renderMode; // NOTE(Brett): GL_TRIANGLES...

	vertex_t *vertices;
	int verticesSz;

private:
	static std::unordered_map<std::string, Mesh*> pool;
	static long   ids;
	static long   currentId;

	bool loaded;
};


//*******************************************************************************************************************
// SPRITE
//*******************************************************************************************************************

enum sprite_direction_t {
	SPRITE_NO_DIRECTION = 0,
	SPRITE_DIRECTION_LEFT,
	SPRITE_DIRECTION_RIGHT,
	SPRITE_DIRECTION_COUNT
};


class Sprite {
public:
	Sprite();

	static Sprite * CreateSprite(const std::string name, 
								 Mesh * const mesh, Texture * const texture,
								 const int frame_columns, const int frame_rows,
								 const int frame_width, const int frame_height,
								 const int total_frames, const int frame_anim_time);

	void Initialize(Mesh * const mesh, Texture * const texture,
					const int frame_columns, const int frame_rows,
					const int frame_width, const int frame_height,
					const int total_frames, const int frame_anim_time );

	void Render(const shader_t * const shader, glm::mat4 *p, glm::mat4 *v) const;
	void Update();

	void StepFrame();
	void SetFrame(const int frame);

public:
	static long ids;

	Timer frameTimer;
	int frameMilliseconds;

	long  id;
	bool  valid;
	float frameWidth;
	float frameHeight;

	int   frameColumns;
	int   frameRows;
	int   currentFrame;
	int   totalFrames;

	sprite_direction_t direction;

	Mesh*    mesh;
	Texture* texture;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 uv;

	std::string name;

private:
	static std::unordered_map<std::string, Sprite*> pool;
};

#endif