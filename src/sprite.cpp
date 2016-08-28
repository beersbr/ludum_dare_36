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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D,
				 0,
				 GL_RGBA,
				 pixels->w, pixels->h, 
				 0,
				 GL_BGRA,
				 GL_UNSIGNED_BYTE,
				 pixels->pixels);

	glBindTexture(GL_TEXTURE_2D, 0);

	width  = pixels->w;
	height = pixels->h;
	loaded = true;
}


void Texture::Use() const {
	if(rsId != currentRsId) {
		currentRsId = rsId;
		glActiveTexture(GL_TEXTURE0+currentTextureUnit);
		glBindTexture(GL_TEXTURE_2D, rsId);
	}
}


//*******************************************************************************************************************
// MESH
//*******************************************************************************************************************

long Mesh::ids          = 0;
long Mesh::currentId    = -1;
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
		#ifdef DEBUG
		std::cout << "binding vao: " << id << std::endl;
		#endif

		currentId = id;
		glBindVertexArray(vaoId);
		// glBindBuffer(GL_ARRAY_BUFFER, vboId);
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

	renderMode = GL_TRIANGLES;

	verticesSz = vertices_sz;
	vertices = (vertex_t*)malloc(sizeof(vertex_t) * verticesSz);
	memcpy((void*)vertices, verts, verticesSz * sizeof(vertex_t));

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


void Mesh::SetRenderMode(GLuint mode) {
	renderMode = mode;
}


//*******************************************************************************************************************
// SPRITE
//*******************************************************************************************************************

long Sprite::ids = 0;

std::unordered_map<std::string, Sprite*> Sprite::pool;

Sprite* Sprite::CreateSprite(const std::string name, 
							 Mesh * const mesh,
							 Texture * const texture,
							 const int frame_columns,
							 const int frame_rows,
							 const int frame_width,
							 const int frame_height,
							 const int total_frames,
							 const int frame_anim_time) {

	Sprite *sprite = new Sprite();

	sprite->Initialize(mesh, texture,
					   frame_columns,
					   frame_rows,
					   frame_width,
					   frame_height,
					   total_frames,
					   frame_anim_time);

	pool[name] = sprite;
	sprite->name = name;
	return sprite;
}


Sprite * Sprite::GetSpriteByName( const std::string name ) {
	return pool[name];
}


Sprite::Sprite() {
	id = ++Sprite::ids;
	valid = false;
}


void Sprite::Initialize(Mesh * const mesh,
						Texture * const texture,
						const int frame_columns,
						const int frame_rows,
						const int frame_width,
						const int frame_height,
						const int total_frames,
						const int frame_anim_time ) {

	this->mesh    = mesh;
	this->texture = texture;

	frameWidth        = frame_width;
	frameHeight       = frame_height; 
	frameColumns      = frame_columns;
	frameRows         = frame_rows;
	currentFrame      = 0;
	totalFrames       = total_frames;
	frameMilliseconds = frame_anim_time;

	direction = SPRITE_DIRECTION_RIGHT;

	position = glm::vec3(0.0f, 0.0f, 0.0f);
	scale    = glm::vec3(1.0f, 1.0f, 1.0f);
	rotation = glm::vec3(0.0f, 0.0f, 0.0f);

	valid = true;
}


void Sprite::Render(glm::mat4 *p, glm::mat4 *v) const {
	mesh->Use();
	glUseProgram(shader.id);

	texture->Use();

	glm::mat4 model = glm::mat4();
	model           = glm::translate(model, position);
	model           = glm::scale(model, scale);


	glm::mat4 uvmodel = glm::mat4();
	
	uvmodel = glm::scale(uvmodel, 
	                     glm::vec3(frameWidth/(float)texture->width,
	                               frameHeight/(float)texture->height,
	                               1.0f)
	                     );

	int frameX = currentFrame % frameColumns;
	int frameY = currentFrame / frameColumns;

	// NOTE(Brett):direction is there so we dont overflow the animation points
	uvmodel = glm::translate(uvmodel,
	                         glm::vec3((float)frameX  + 1 * direction,
	                                   (float)frameY,
	                                   0.0f)
	                         );

	uvmodel = glm::rotate(uvmodel, 3.14159f * direction, glm::vec3(0.0f, 1.0f, 0.0f));


	GLint sampler0UniformLocation   = glGetUniformLocation(shader.id, "sampler0");
	GLint modelUniformLocation      = glGetUniformLocation(shader.id, "model");
	GLint uvmodelUniformLocation    = glGetUniformLocation(shader.id, "uvmodel");
	GLint projectionUniformLocation = glGetUniformLocation(shader.id, "projection");
	GLint viewUniformLocation       = glGetUniformLocation(shader.id, "view");

	glUniformMatrix4fv(projectionUniformLocation, 1, GL_FALSE, (GLfloat*)p);
	glUniformMatrix4fv(viewUniformLocation,       1, GL_FALSE, (GLfloat*)v);

	glUniform1i(sampler0UniformLocation,       texture->currentTextureUnit);
	glUniformMatrix4fv(modelUniformLocation,   1, GL_FALSE, (GLfloat*)&model[0]);
	glUniformMatrix4fv(uvmodelUniformLocation, 1, GL_FALSE, (GLfloat*)&uvmodel[0]);

	glDrawArrays(GL_TRIANGLES, 0, mesh->verticesSz);
}


void Sprite::Update() {
	if(frameMilliseconds > 0) {
		if(frameTimer.Stopwatch(frameMilliseconds)) {
			StepFrame();
		}	
	}
	
}


void Sprite::StepFrame() {
	SetFrame(currentFrame + 1);
}


void Sprite::SetFrame(const int frame) {
	currentFrame = frame % totalFrames;
}


