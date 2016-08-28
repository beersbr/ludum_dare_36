#include "engine.hpp"

LD36Engine::LD36Engine() {
	running = false;
	valid   = false;
	paused  = false;
}

LD36Engine::~LD36Engine() {

}

void LD36Engine::Initialize(const int window_width, const int window_height, const char * const title) {
	windowWidth = window_width;
	windowHeight = window_height;

	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		#ifdef DEBUG
		std::cout << "Could not initialize SDL" << std::endl;
		#endif
		return;
	}


	if(!IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG|IMG_INIT_TIF)) {
		#ifdef DEBUG
		std::cout << "Could not initialize sdl image" << std::endl;
		#endif
		return;
	}

	window = SDL_CreateWindow(title, 
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              windowWidth,
                              windowHeight,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	if(!window) {
		#ifdef DEBUG
		std::cout << "Could not create main window" << std::endl;
		#endif
		return;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	context = SDL_GL_CreateContext(window);

	#ifdef _WIN32
	GLenum err = glewInit();
	#endif

	if(!context) {
		#ifdef DEBUG
		std::cout << "Could not init gl" << std::endl;
		#endif
		return;
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	SDL_GL_SetSwapInterval(1);

	running = true;
	valid   = true;

}


bool LD36Engine::IsValid() const {
	return valid;
}


void LD36Engine::Update() {
	static Timer entityTimedUpdate;
	static Timer timedUpdate;
	static long currentTime      = timedUpdate.GetTimer();
	static long lastEntityUpdate = currentTime;

	currentTime = timedUpdate.GetTimer();


	coreFrameCount += 1;
	if(coreTimer.Stopwatch(999)) { 
		coreFrameRate = coreFrameCount;
		coreFrameCount = 0;
		#ifdef DEBUG
		std::cout << "core frame rate: " << coreFrameRate << std::endl;
		#endif 
	}

	if(entityTimedUpdate.Stopwatch(1000/120.0f)) {
		game->Update(currentTime - lastEntityUpdate);
		game->EntityUpdate(currentTime - lastEntityUpdate);
		lastEntityUpdate = currentTime;
		Keyboard::Instance()->ClearKeyPressedStates();

		std::list<GameObject *>::iterator iter = objects.begin();
		while(iter != objects.end()) {
			if((*iter)->alive == false) {
				iter = objects.erase(iter);
			}
			else {
				iter++;
			}
		}
	}

	// NOTE(Brett):Gives about 60 fps
	if(!timedUpdate.Stopwatch(1000/60.0f)) {
		usleep(1);
	}
	else {
	// if(timedUpdate.Stopwatch(15)) {
		realFrameCount += 1;
		if(realTimer.Stopwatch(999)) {
			realFrameRate = realFrameCount;
			realFrameCount = 0;
			std::cout << "real frame rate: " << realFrameRate << std::endl;
		}

		game->Render();	
		SDL_GL_SwapWindow(window);
	}

}


bool LD36Engine::IsRunning() const {
	return running;
}


void LD36Engine::Shutdown() {
	running = false;
}

