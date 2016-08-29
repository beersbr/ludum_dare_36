#include "eye_guy_game_object.hpp"

EyeGuyGameObject::EyeGuyGameObject() : GameObject() {

}


EyeGuyGameObject::~EyeGuyGameObject() {

}


void EyeGuyGameObject::Update(const long elapsedMilliseconds) {
	

	if(!dying) {
		sprite.SetFrame(0);
		// NOTE(Brett):Fly towards the player 
		glm::vec3 delta = Engine->player->position - position;
		glm::vec3 direction = glm::normalize(delta);
		float elapsedSeconds = elapsedMilliseconds/1000.f;
		velocity = velocity * 0.82f;
		velocity += (direction * 28.0f) * elapsedSeconds;

		sprite.direction = (velocity.x >= 0) ? SPRITE_DIRECTION_RIGHT : SPRITE_DIRECTION_LEFT;	
	}
	else {
		if(dyingTimer.Stopwatch(500))
			alive = false;
	}
	
}


void EyeGuyGameObject::Render(glm::mat4 *p, glm::mat4 *v) {
	GameObject::Render(p, v);
}


void EyeGuyGameObject::CollisionWith(GameObject *collisionTarget) {
	switch( collisionTarget->objectName ) {
		case OBJECT_NAME_PLAYER:
		{
			break;
		}
		case OBJECT_NAME_DASH:
		{
			sprite.SetFrame(1);
			if(!dying){
				dying = true;
				dyingTimer.Reset();	
			}
			
			break;
		}
		default:
		{
			break;
		}
	}

}
