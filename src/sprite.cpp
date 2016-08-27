#include "sprite.hpp"

//*******************************************************************************************************************
// TEXTURE
//*******************************************************************************************************************

long Texture::ids = 0;
GLuint Texture::currentRsId = 0;
std::unordered_map<std::string, Texture*> Texture::pool;

Texture* Texture::CreateTexture(const std::string name, const char * const filename) {
	Texture *texture = new Texture();
	texture->Load(filename);

	Texture::pool[name] = texture;
	return texture;
}


Texture::Texture() {
	pixels             = nullptr;
	rsId               = 0;
	loaded             = false;
	currentTextureUnit = 0;
	id                 = ++Texture::ids;
}


Texture::~Texture() {
	SDL_FreeSurface(pixels);
}


void Texture::Load(const char * const filename) {
	pixels = IMG_Load(filename);
	if(!pixels) {
		#ifdef DEBUG
		std::cout << "Could not load texture named " << filename << std::endl;
		#endif
		return;
	}

	glGenTextures(1, &rsId);
	glBindTexture(GL_TEXTURE_2D, rsId);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D,
	             0,
	             GL_RGBA,
	             pixels->w, pixels->h, 
	             0,
	             GL_BGRA,
	             GL_UNSIGNED_BYTE,
	             pixels->pixels);

	glBindTexture(GL_TEXTURE_2D, 0);

	size   = glm::vec2(pixels->w, pixels->h);
	loaded = true;
}


void Texture::Use() const {
	if(rsId != currentRsId) {
		currentRsId = rsId;
		// glActiveTexture(GL_TEXTURE0+currentTextureUnit);
		glBindTexture(GL_TEXTURE_2D, rsId);
	}
}


//*******************************************************************************************************************
// MESH
//*******************************************************************************************************************

long Mesh::ids          = 0;
long Mesh::currentId    = 0;
std::unordered_map<std::string, Mesh*> Mesh::pool;


Mesh * Mesh::CreateMesh(const std::string name, const vertex_t * const vertices, const int vertices_sz ) {
	Mesh* mesh = new Mesh();
	mesh->Initialize(vertices, vertices_sz);

	Mesh::pool[name] = mesh;
	return mesh;
}


Mesh::Mesh() {
	loaded = false;
	id = ++Mesh::ids;
}


Mesh::~Mesh() {
	// TODO(Brett):free the stuff
}


void Mesh::Use() const {
	if(Mesh::currentId != id) {
		currentId = id;
	}
}


void Mesh::Initialize(const vertex_t * const verts, const int vertices_sz) {
	glGenVertexArrays(1, &vaoId);
	if(vaoId == 0) {
		#ifdef DEBUG
		std::cout << "Could not create VAO in Mesh" << glGetError() << std::endl;
		#endif
		return;
	}

	glBindVertexArray(vaoId);

	glGenBuffers(1, &vboId);

	if(vboId == 0) {
		#ifdef DEBUG
		std::cout << "Could not create VBO in Mesh: " << glGetError() << std::endl;
		#endif
	}

	verticesSz = vertices_sz;
	vertices = (vertex_t*)malloc(sizeof(vertex_t) * verticesSz);
	memcpy((void*)vertices, verts, verticesSz);

	glBindBuffer(GL_ARRAY_BUFFER, vboId);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, position));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, color));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, uv));
 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_t) * verticesSz, (GLvoid*)vertices, GL_STATIC_DRAW);

	glBindVertexArray(0);

	loaded = true;
}


//*******************************************************************************************************************
// SPRITE
//*******************************************************************************************************************

long Sprite::ids = 0;

Sprite::Sprite() {
	id = ++Sprite::ids;
}


void Sprite::Render() const {

}


void Sprite::StepFrame() {
	SetFrame(currentFrame + 1);
}


void Sprite::SetFrame(const int frame) {
	currentFrame = frame % totalFrames;
}


