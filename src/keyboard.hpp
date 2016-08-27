#ifndef _LD_KEYBOARD_
#define _LD_KEYBOARD_

#include <SDL2/SDL.h>
#include <iostream>
#include <cstdlib>
#include <unordered_map>

class Keyboard {
public:
	static Keyboard * const Instance();
	void   Keydown(const SDL_Keycode code);
	void   Keyup(const SDL_Keycode code);
	void   SetKeyPressed(const SDL_Keycode code);
	void   ClearKeyPressedStates();
	void   ClearKeyStates();

	bool   KeyIsDown(SDL_Keycode code);
	bool   KeyWasPressed(SDL_Keycode code);

private:
	Keyboard();

	static Keyboard *instance;

	std::unordered_map<SDL_Keycode, bool> keyStates;
	std::unordered_map<SDL_Keycode, bool> keyPressedStates;
};

#endif