
#include "ld36game.hpp"
void LD36Game::End() {

}

void LD36Game::Preload() {
	Texture* texture        = Texture::CreateTexture("sheet1", "images/sheet1.png");
	Texture* walkingTexture = Texture::CreateTexture("walking", "images/walking.png");
	Texture* dashTexture    = Texture::CreateTexture("dash", "images/dash.png");
	Texture* blockTexture   = Texture::CreateTexture("block", "images/block.png");

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

	Sprite::CreateSprite("block", mesh, blockTexture,
	                     1.0, 1.0,
	                     50.0f, 50.0f,
	                     1,
	                     10000);


	srand(SDL_GetTicks());
	Player *player        = new Player();
	player->sprite        = *Sprite::GetSpriteByName("basic");
	player->sprite.shader = shader;
	player->scale         = glm::vec3(120.0f, 60.0f, 1.0f);
	player->position      = glm::vec3(500.0f, 500.0f, 0.0f);

	Engine->objects.push_back(player);


	for(auto i = 0; i < 24; i++) {
		GameObject *block    = new GameObject();
		block->sprite        = *Sprite::GetSpriteByName("block");
		block->sprite.shader = shader;
		block->scale         = glm::vec3(50.0f, 50.0f, 1.0f);
		block->position      = glm::vec3(i * 50.0f +  25.0f, 25.0f, -0.5f);
		block->objectName    = OBJECT_NAME_BLOCK;
		block->objectType    = OBJECT_TYPE_SOLID;

		Engine->objects.push_back(block);
	}

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
				if((*iter)->velocity.y != 0)
					(*iter)->onGround = false;

				glm::vec3 gravity = glm::vec3(0.0f, 0.0f, 0.0f);
				if(!(*iter)->onGround)
					gravity = glm::vec3(0.0f, (-500.0f * (float)elapsedSeconds), 0.0f);

				(*iter)->position = (*iter)->position + (*iter)->velocity + gravity;

				std::list<GameObject *>::iterator collisionIter = Engine->objects.begin();

				rect_t currentEntityRect = (*iter)->GetRect();

				while(collisionIter != Engine->objects.end()) {
					if(iter == collisionIter) {
						collisionIter++;
						continue;
					}

					rect_t targetEntityRect = (*collisionIter)->GetRect();

					if(rectanglesCollide(currentEntityRect, targetEntityRect)) {
						// std::cout << "collision!" << std::endl;

						// std::cout << "target rect: " << targetEntityRect.x << ", " << targetEntityRect.y << ", " << targetEntityRect.width << ", " << targetEntityRect.height << std::endl;
						// std::cout << "current rect: " << currentEntityRect.x << ", " << currentEntityRect.y << ", " << currentEntityRect.width << ", " << currentEntityRect.height << std::endl;


						(*iter)->CollisionWith((*collisionIter));
						(*collisionIter)->CollisionWith((*iter));
					} //if

					collisionIter++;

				} //while

			} // default

		} //switch

	} //for

}


void LD36Game::Render() {
	glClearColor(0.1f, 0.5f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	std::list<GameObject *>::iterator iter;
	for(iter = Engine->objects.begin(); iter != Engine->objects.end(); iter++) {
		(*iter)->Render(&Engine->projectionMatrix, &Engine->viewMatrix);
	}
}

