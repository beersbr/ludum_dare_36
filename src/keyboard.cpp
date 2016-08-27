
#include "keyboard.hpp"

//*******************************************************************************************************************
// STATIC
//*******************************************************************************************************************

Keyboard * Keyboard::instance = nullptr;

Keyboard * const Keyboard::Instance() {
	if(!Keyboard::instance) {
		Keyboard::instance = new Keyboard();
	}
	return Keyboard::instance;
}

//*******************************************************************************************************************
// FUNCTIONS
//*******************************************************************************************************************

Keyboard::Keyboard() {
	
}

void Keyboard::Keydown( const SDL_Keycode code ) {
	keyStates[code] = true;
}

void Keyboard::Keyup( const SDL_Keycode code ) {
	keyStates[code] = false;
}

void Keyboard::SetKeyPressed( const SDL_Keycode code ) {
	keyPressedStates[code] = true;
}


void Keyboard::ClearKeyPressedStates() {
	keyPressedStates.clear();
}


void Keyboard::ClearKeyStates() {
	keyStates.clear();
}

bool Keyboard::KeyIsDown(SDL_Keycode code) {
	return (!!keyStates[code]);
}


bool Keyboard::KeyWasPressed(SDL_Keycode code) {
	return (!!keyPressedStates[code]);
}