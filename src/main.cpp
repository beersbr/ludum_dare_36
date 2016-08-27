
#include "common.hpp"
#include "common.cpp"

#include "keyboard.hpp"
#include "keyboard.cpp"

#ifdef _WIN32
#include <GL/glew.h>
#endif


int main(int argc, char* argv[]) {

	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "Could not initialize SDL" << std::endl;
		return INITIALIZATION_ERROR;
	}


	if(!IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG|IMG_INIT_TIF)) {
		std::cout << "Could not initialize sdl image" << std::endl;
		return INITIALIZATION_ERROR;
	}


	SDL_Window *mainWindow = SDL_CreateWindow(EXE_NAME, 
	                                          SDL_WINDOWPOS_CENTERED,
	                                          SDL_WINDOWPOS_CENTERED,
	                                          WINDOW_WIDTH,
	                                          WINDOW_HEIGHT,
	                                          SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	if(!mainWindow) {
		std::cout << "Could not create main window" << std::endl;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GLContext mainContext = SDL_GL_CreateContext(mainWindow);

	#ifdef _WIN32
	GLenum err = glewInit();
	#endif

	if(!mainContext){
		std::cout << "Could not init gl" << std::endl;
	}


	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	SDL_GL_SetSwapInterval(1);


	spriteSheet_t sheet1 = loadSpriteSheet("images/sheet1.png");

	if(!sheet1.loaded) {
		std::cout << "Could not load the sprite sheet" << std::endl;
		return -1;
	}

	shader_t spriteShader = loadShader("shaders/sprite.vertex.glsl", "shaders/sprite.fragment.glsl", "sprite");

	sprite_t sprite = createSprite(&sheet1,
	                               &spriteShader,
	                               glm::vec2(0.0f, 50.0f),
	                               glm::vec2(50.0f, 50.0f),
	                               3);



	sprite.scale = glm::vec3(50.0f, 50.0f, 1.0f);
	sprite.position = glm::vec3(25.0f, 25.0f, 0.0f);

	SDL_Event event = {};
	bool running = true;
	long startTime = SDL_GetTicks();
	long runningMilliseconds = 0;
	long currentMilliseconds = 0;
	long lastMilliseconds = 0;
	long elapsedMilliseconds = 0;
	long frames = 0;

	glm::mat4 renderProjectionMatrix = glm::ortho(0.0f, (float)WINDOW_WIDTH, 0.0f, (float)WINDOW_HEIGHT, 0.1f, 100.f);
	glm::mat4 renderViewMatrix = glm::mat4();

	float stepTimeSwap = 1/30.f;
	float stepTime = stepTimeSwap;

	while(running) {
		long lastMilliseconds = currentMilliseconds;
		currentMilliseconds = SDL_GetTicks();
		elapsedMilliseconds = currentMilliseconds - lastMilliseconds;
		float elapsedSeconds = elapsedMilliseconds/1000.0f;

		while(SDL_PollEvent(&event)) {
				switch(event.type) {
					case SDL_KEYDOWN:
					{
						Keyboard::Instance()->Keydown(event.key.keysym.sym);
						break;
					}
					case SDL_KEYUP:
					{
						Keyboard::Instance()->Keyup(event.key.keysym.sym);
						Keyboard::Instance()->SetKeyPressed(event.key.keysym.sym);
						break;
					}
					case SDL_WINDOWEVENT:
					{
						switch(event.window.type) {
							case SDL_WINDOWEVENT_CLOSE: 
							{
								running = false;
								break;
							}
						}
						break;
					}
					case SDL_QUIT:
					{
						running = false;
						break;
					}
				}
		}

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderSprite(&renderProjectionMatrix, &renderViewMatrix, &sprite);

		stepTime -= elapsedSeconds;
		if(stepTime <= 0.0){
			stepSpriteFrame(&sprite);
			stepTime = stepTimeSwap - (stepTime - stepTimeSwap);
		}


		if(Keyboard::Instance()->KeyIsDown(SDLK_UP)) {
			sprite.position.y += 100.f * elapsedSeconds;
		}
		if(Keyboard::Instance()->KeyIsDown(SDLK_DOWN)) {
			sprite.position.y -= 100.f * elapsedSeconds;
		}
		if(Keyboard::Instance()->KeyIsDown(SDLK_LEFT)) {
			sprite.position.x -= 100.f * elapsedSeconds;
		}
		if(Keyboard::Instance()->KeyIsDown(SDLK_RIGHT)) {
			sprite.position.x += 100.f * elapsedSeconds;
		}

		



		SDL_GL_SwapWindow(mainWindow);

		frames += 1;
		Keyboard::Instance()->ClearKeyPressedStates();
	}

	return 0;
}