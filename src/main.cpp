
#include "common.hpp"
#include "common.cpp"

#include "keyboard.hpp"

#include "engine.hpp"
#include "engine.cpp"

#include "ld36game.hpp"
#include "ld36game.cpp"

#ifdef _WIN32
#include <GL/glew.h>
#endif

LD36Engine* Engine;

int main(int argc, char* argv[]) {

	Engine = new LD36Engine();
	Engine->Initialize(WINDOW_WIDTH, WINDOW_HEIGHT, EXE_NAME);

	Engine->game = new LD36Game();

	Engine->game->Preload();

	SDL_Event event = {};
	while(Engine->IsRunning()) {
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
								Engine->running = false;
								break;
							}
						}
						break;
					}
					case SDL_QUIT:
					{
						Engine->running = false;
						break;
					}
				}
		}

		Engine->Update();
	}

	return 0;
}