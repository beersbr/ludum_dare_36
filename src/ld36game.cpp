#include "ld36game.hpp"

void LD36Game::End() {

}

void LD36Game::Preload() {
	Texture* texture = Texture::CreateTexture("sheet1", "images/sheet1.png");

	vertex_t verts[6] = {
		{ glm::vec3(-0.5f,  0.5f,  -0.5f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(-0.5f, -0.5f,  -0.5f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3( 0.5f,  0.5f,  -0.5f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3( 0.5f,  0.5f,  -0.5f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(-0.5f, -0.5f,  -0.5f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3( 0.5f, -0.5f,  -0.5f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f) }
	};

	Mesh* mesh = Mesh::CreateMesh("tile", (vertex_t*)&verts[0], 6);

	projectionMatrix = glm::ortho(0.0f, (float)Engine->windowWidth, 0.0f, (float)Engine->windowHeight, 0.1f, 100.0f);
	viewMatrix       = glm::mat4();

	shader = loadShader("shaders/sprite.vertex.glsl", "shaders/sprite.fragment.glsl", "sprite");

	sprite = Sprite::CreateSprite("basic", mesh, texture,
	                              8.0f, 8.0f,
	                              50.0f, 50.0f,
	                              3,
	                              30);


	sprite->scale    = glm::vec3(200.0f, 200.0f, 1.0f);
	sprite->position = glm::vec3(200.0f, 200.0f, 0.0f);

}


void LD36Game::Update() {

}


void LD36Game::Render() {
	glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	sprite->Render(&shader, &projectionMatrix, &viewMatrix);

}

