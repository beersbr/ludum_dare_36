#include "ld36game.hpp"


void LD36Game::End() {

}


void LD36Game::Preload() {
	Texture::CreateTexture("sheet1", "images/sheet1.png");
}


void LD36Game::Update() {

}


void LD36Game::Render() {
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

