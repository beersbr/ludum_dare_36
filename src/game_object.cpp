#include "game_object.hpp"


GameObject::GameObject() {
	lifeTimeLength = 0;
	visible        = true;
	alive          = true;
	position       = glm::vec3(0.0f, 0.0f, 0.0f);
	rotation       = glm::vec3(0.0f, 0.0f, 0.0f);
	scale          = glm::vec3(0.0f, 0.0f, 0.0f);
	velocity       = glm::vec3(0.0f, 0.0f, 0.0f);
	acceleration   = glm::vec3(0.0f, 0.0f, 0.0f);
	objectType     = OBJECT_TYPE_NONE;
	objectName     = OBJECT_NAME_NONE;
}


GameObject::~GameObject() {

}


void GameObject::Update(const long elapsedMilliseconds) {
	sprite.Update();

	if(lifeTimeLength > 0 && lifeTimeTimer.Stopwatch(lifeTimeLength)) {
		alive = false;
	}

}


void GameObject::Render(glm::mat4 *p, glm::mat4 *v) {
	sprite.scale = this->scale;
	sprite.position = this->position;
	sprite.Render(p, v);
}


void GameObject::CollisionWith(GameObject *collisionTarget) {


}


rect_t const GameObject::GetRect() const {
	rect_t rectangle = { .x = position.x - scale.x/2.f,
						 .y = position.y - scale.y/2.f,
						 .width = scale.x,
						 .height = scale.y };
	return rectangle;


}