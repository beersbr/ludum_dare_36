#include "ld36game.hpp"

void LD36Game::End() {

}

void LD36Game::Preload() {
	Texture* texture        = Texture::CreateTexture("sheet1", "images/sheet1.png");
	Texture* walkingTexture = Texture::CreateTexture("walking", "images/walking.png");
	Texture* dashTexture    = Texture::CreateTexture("dash", "images/dash.png");

	vertex_t verts[6] = {
		{ glm::vec3(-0.5f,  0.5f,  -0.5f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(-0.5f, -0.5f,  -0.5f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3( 0.5f,  0.5f,  -0.5f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3( 0.5f,  0.5f,  -0.5f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(-0.5f, -0.5f,  -0.5f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3( 0.5f, -0.5f,  -0.5f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f) }
	};

	Mesh* mesh = Mesh::CreateMesh("tile", (vertex_t*)&verts[0], 6);

	Engine->projectionMatrix = glm::ortho(0.0f, (float)Engine->windowWidth, 0.0f, (float)Engine->windowHeight, 0.1f, 100.0f);
	Engine->viewMatrix       = glm::mat4();

	shader = loadShader("shaders/sprite.vertex.glsl", "shaders/sprite.fragment.glsl", "sprite");

	Sprite::CreateSprite("dash", mesh, dashTexture,
	                     1.0f, 4.0f,
	                     200.0f, 50.0f,
	                     4,
	                     20);

	Sprite::CreateSprite("basic", mesh, walkingTexture,
	                     6.0, 1.0,
	                     60.0f, 120.0f,
	                     6,
	                     250);

	srand(SDL_GetTicks());

	Player *player        = new Player();
	player->sprite        = *Sprite::GetSpriteByName("basic");
	player->sprite.shader = shader;
	player->scale         = glm::vec3(120.0f, 60.0f, 1.0f);
	player->position      = glm::vec3(500.0f, 500.0f, 0.0f);


	Engine->objects.push_back(player);

	// for(auto i = 0; i < 2000; i++) {
	// 	GameObject *gameObject = new GameObject();
	// 	gameObject->position = glm::vec3(RANDOM((float)Engine->windowWidth),
	// 	                                 RANDOM((float)Engine->windowHeight),
	// 	                                 0.0f);

	// 	float scale = RANDOM(40.f) + 10.f;
	// 	gameObject->scale = glm::vec3(scale, scale, 1.f);

	// 	gameObject->sprite = *Sprite::GetSpriteByName("dash");
	// 	gameObject->sprite.shader = shader;
	// 	gameObject->sprite.frameMilliseconds = RANDOM(50) + 100;

	// 	gameObject->velocity = glm::vec3(RANDOM(5.f) + -2.5f,
	// 	                                 RANDOM(5.f) + -2.5f,
	// 	                                 0.0f);


	// 	Engine->objects.push_back(gameObject);
	// }
}


void LD36Game::Update(const long elapsedMilliseconds) {

}


void LD36Game::EntityUpdate(const long elapsedMilliseconds) {
	std::list<GameObject *>::iterator iter;

	float elapsedSeconds = elapsedMilliseconds/1000.0;

	for(iter = Engine->objects.begin(); iter != Engine->objects.end(); iter++) {
		(*iter)->Update(elapsedMilliseconds);

		switch((*iter)->objectType) {
			case OBJECT_TYPE_SOLID:
			{
				break;
			}
			default:
			{
				(*iter)->position = (*iter)->position + (*iter)->velocity + glm::vec3(0.0f, (-500.0f * (float)elapsedSeconds), 0.0f);
			}
		}

	}
}


void LD36Game::Render() {
	glClearColor(0.1f, 0.5f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	std::list<GameObject *>::iterator iter;
	for(iter = Engine->objects.begin(); iter != Engine->objects.end(); iter++) {
		(*iter)->Render(&Engine->projectionMatrix, &Engine->viewMatrix);
	}
}

