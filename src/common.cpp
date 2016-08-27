#include "common.hpp"

char * const loadBinaryFile(const char * const filename) {
	FILE *fp = fopen(filename, "rb+");
	if(!fp) {
		#ifdef DEBUG
		std::cout << "Could not open file named " << filename << std::endl;
		#endif
		return 0;
	}
	fseek(fp, 0, SEEK_END);
	long flength = ftell(fp);

	fseek(fp, 0, SEEK_SET);

	#ifdef DEBUG
	std::cout << "Size of file " << filename << " " << flength << std::endl;
	#endif

	char *data = (char*)malloc(sizeof(char)*flength+1);
	long read = fread(data, sizeof(char), flength+1, fp);

	if(read != flength) {
		#ifdef DEBUG
		std::cout << "Warning: did not read entire file named " << filename << std::endl;
		#endif
		free(data);
		return 0;
	}

	data[flength] = '\0';

	return data;
}


shader_t loadShader(const char * const vertex_name, const char * const fragment_name, const char * const name) {

	char *vertexData = loadBinaryFile(vertex_name);
	char *fragmentData = loadBinaryFile(fragment_name);

	#ifdef DEBUG
	std::cout << ">> read vertex " << vertex_name << std::endl << vertexData <<std::endl;
	std::cout << ">> read fragment " << fragment_name << std::endl << fragmentData <<std::endl;
	#endif

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, &vertexData, 0);
	glCompileShader(vertexShader);

	GLint compilationResult = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compilationResult);

	if(!compilationResult) {
		GLint logLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);
		char *log = (char*)malloc(sizeof(char)*logLength);
		glGetShaderInfoLog(vertexShader, logLength, 0, (GLchar*)log);

		std::cout << ">> Vertex shader compilation issue: " << log << std::endl;
	}

	glShaderSource(fragmentShader, 1, &fragmentData, 0);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compilationResult);

	if(!compilationResult) {
		GLint logLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);
		char *log = (char*)malloc(sizeof(char)*logLength);
		glGetShaderInfoLog(fragmentShader, logLength, 0, (GLchar*)log);

		std::cout << ">> Fragment shader compilation issue: " << log << std::endl;
	}

	GLuint shaderId = glCreateProgram();
	if(!shaderId) {
		#ifdef DEBUG
		std::cout << "Could not create a shader program:" << glGetError() << std::endl;
		#endif
	}


	glAttachShader(shaderId, vertexShader);
	glAttachShader(shaderId, fragmentShader);
	glLinkProgram(shaderId);

	GLint linkResult;

	glGetProgramiv(shaderId, GL_LINK_STATUS, &linkResult);
	if(!linkResult) {
		#ifdef DEBUG
		std::cout << "Coud not link program: " << glGetError() << std::endl;
		#endif
	}

	shader_t shader = { .id = shaderId };
	strncpy(shader.name, name, RESOURCE_NAME_SZ-1);

	return shader;
}


renderable_t * createRenderable( mesh_t * const mesh ) {
	std::cout << "ERROR: " << glGetError() << std::endl;
	static long ids = 0;

	renderable_t * const renderable = (renderable_t*)malloc(sizeof(renderable_t));
	renderable->id = ++ids;
	renderable->mesh = mesh;

	glGenVertexArrays(1, &renderable->vao);

	if(renderable->vao == 0) {
		#ifdef DEBUG
		std::cout << "Could not create VAO for id " << renderable->id << std::endl;
		#endif
		return {};
	}

	glBindVertexArray(renderable->vao);

	glGenBuffers(1, &renderable->vbo);

	if(renderable->vbo == 0) {
		#ifdef DEBUG
		std::cout << "Could not create VBO for id " << renderable->id << std::endl;
		#endif
		return {};
	}

	glBindBuffer(GL_ARRAY_BUFFER, renderable->vbo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, position));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, color));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, uv));
 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_t) * mesh->verticesSz, (GLvoid*)mesh->vertices, GL_STATIC_DRAW);

	glBindVertexArray(0);

	return renderable;

}


void bindRenderable(renderable_t * const renderable) {
	static long currentRenderableId = 0;

	if(renderable->id != currentRenderableId) {
		glBindVertexArray(renderable->vao);
	}
}


spriteSheet_t loadSpriteSheet( const char * const filename) {
	static long ids = 0;
	SDL_Surface *image = IMG_Load(filename);
	if(!image) {
		return {};
	}

	spriteSheet_t spriteSheet = { .id = ++ids };
	glGenTextures(1, &spriteSheet.ident);
	glBindTexture(GL_TEXTURE_2D, spriteSheet.ident);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D,
	             0,
	             GL_RGBA,
	             image->w, image->h, 
	             0,
	             GL_BGRA,
	             GL_UNSIGNED_BYTE,
	             image->pixels);

	glBindTexture(GL_TEXTURE_2D, 0);

	spriteSheet.size = glm::vec2(image->w, image->h);
	spriteSheet.loaded = (spriteSheet.ident != 0);

	return spriteSheet;
}


sprite_t createSprite( spriteSheet_t * const sprite_sheet, shader_t * const shader, glm::vec2 origin, glm::vec2 frame_size, int frame_count ) {
	sprite_t sprite = { .spriteSheet = sprite_sheet, .frameOrigin = origin, .frameSize = frame_size, .framesTotal = frame_count };
	sprite.renderable = createRenderable(getTileMesh());

	sprite.frameScale = glm::vec3(sprite.frameSize.x/sprite.spriteSheet->size.x,
	                              sprite.frameSize.y/sprite.spriteSheet->size.y,
	                              1.0f);
	sprite.uvmodel = glm::mat4();
	sprite.uvmodel = glm::translate(sprite.uvmodel, glm::vec3(sprite.frameOrigin.x, sprite.frameOrigin.y, 0.0f));
	sprite.uvmodel = glm::scale(sprite.uvmodel, sprite.frameScale);

	sprite.shader = shader;

	return sprite;
}


void renderSprite( const glm::mat4 * const projectionMatrix, const glm::mat4 * const viewMatrix, const sprite_t * const sprite ) {

	glm::mat4 spriteModel = glm::mat4();
	spriteModel = glm::translate(spriteModel, sprite->position);
	spriteModel = glm::scale(spriteModel, sprite->scale);

	glUseProgram(sprite->shader->id);
	bindRenderable(sprite->renderable);
	glBindTexture(GL_TEXTURE_2D, sprite->spriteSheet->ident);

	GLuint sampler0Location   = glGetUniformLocation(sprite->shader->id, "sampler0");
	GLuint modelLocation      = glGetUniformLocation(sprite->shader->id, "model");
	GLuint uvMatrixLocation   = glGetUniformLocation(sprite->shader->id, "uvmodel");
	GLuint projectionLocation = glGetUniformLocation(sprite->shader->id, "projection");
	GLuint viewLocation       = glGetUniformLocation(sprite->shader->id, "view");

	glUniform1i(sampler0Location,          0);
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, (GLfloat*)&projectionMatrix[0]);
	glUniformMatrix4fv(viewLocation,       1, GL_FALSE, (GLfloat*)&viewMatrix[0]);
	glUniformMatrix4fv(modelLocation,      1, GL_FALSE, (GLfloat*)&spriteModel[0]);
	glUniformMatrix4fv(uvMatrixLocation,   1, GL_FALSE, (GLfloat*)&sprite->uvmodel[0]);

	glDrawArrays(sprite->renderable->mesh->renderMode, 0, sprite->renderable->mesh->verticesSz);

}


void setSpriteFrame( sprite_t * const sprite, const int frame ) {
	sprite->currentFrame = (frame % sprite->framesTotal);
	sprite->uvmodel = glm::mat4();
	sprite->uvmodel = glm::scale(sprite->uvmodel, sprite->frameScale);
	sprite->uvmodel = glm::translate(sprite->uvmodel, glm::vec3(sprite->currentFrame,
	                                                            sprite->frameOrigin.y/sprite->spriteSheet->size.x/sprite->frameScale.y,
	                                                            0.0f));

	// TODO(Brett):This is a dumb....
	// std::cout << sprite->frameOrigin.y/sprite->spriteSheet->size.x/sprite->frameScale.y << std::endl;
}


void stepSpriteFrame(sprite_t * const sprite ) {
	setSpriteFrame(sprite, sprite->currentFrame+1);
}


mesh_t * getTileMesh() {
	static mesh_t * TILE_MESH = nullptr;

	if(!TILE_MESH){
		TILE_MESH = (mesh_t*)malloc(sizeof(mesh_t));
		TILE_MESH->renderMode = GL_TRIANGLES;
		TILE_MESH->verticesSz = 6;
		TILE_MESH->vertices = (vertex_t*)malloc(sizeof(vertex_t)*TILE_MESH->verticesSz);

		TILE_MESH->vertices[0] = { glm::vec3(-0.5f,  0.5f,  -0.5f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f) };
		TILE_MESH->vertices[1] = { glm::vec3(-0.5f, -0.5f,  -0.5f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f) };
		TILE_MESH->vertices[2] = { glm::vec3( 0.5f,  0.5f,  -0.5f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f) };
		TILE_MESH->vertices[3] = { glm::vec3( 0.5f,  0.5f,  -0.5f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f) };
		TILE_MESH->vertices[4] = { glm::vec3(-0.5f, -0.5f,  -0.5f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f) };
		TILE_MESH->vertices[5] = { glm::vec3( 0.5f, -0.5f,  -0.5f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f) };
	}
	return TILE_MESH;
}


gameObjectManager_t createGameObjectManager(const long max_objects_count) {
	gameObjectManager_t manager = { .maxObjectsCount = max_objects_count, .poolSize = max_objects_count * (long)sizeof(gameObject_t) };
	manager.pool = malloc(manager.poolSize);
	manager.gameObjects = (gameObject_t*)&manager.pool;
	return manager;
}