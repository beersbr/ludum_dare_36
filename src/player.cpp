#include "player.hpp"

Player::Player() : GameObject() {
	shouldTakeDamage = true;
}


Player::~Player() {

}


void Player::Update(const long elapsedMilliseconds) {

	static Timer dashTimer;
	static bool canDash = true;

	static Timer onGroundTimer;

	if(!onGround){
		if(onGroundTimer.Stopwatch(1500)) {
			position = glm::vec3(500.0f, 500.0f, 0.0f);	
		}
	} else {
		onGroundTimer.Reset();
	}

	sprite.Update();

	int direction = 0;

	acceleration = glm::vec3(0.0f, 0.0f, 0.0f);

	if(onGround) {
		if(Keyboard::Instance()->KeyWasPressed(SDLK_w)) {
			acceleration.y = 3300.f;
		}
	}

	if(Keyboard::Instance()->KeyIsDown(SDLK_s)) {
		acceleration.y = -75.f;
	}
	if(Keyboard::Instance()->KeyIsDown(SDLK_a)) {
		acceleration.x = -75.f;
		direction = -1;
	}
	if(Keyboard::Instance()->KeyIsDown(SDLK_d)) {
		acceleration.x = 75.f;
		direction = 1;
	}

	if(!canDash) {
		if(dashTimer.Stopwatch(600)) {
			canDash = true;
			shouldTakeDamage = true;
		}
	} else {
		if(Keyboard::Instance()->KeyWasPressed(SDLK_j) && direction != 0) {
			shouldTakeDamage = false;
			acceleration.x = direction * 3300.f;
			canDash = false;

			GameObject *dash               = new GameObject();
			dash->lifeTimeLength           = 160;
			dash->sprite                   = *Sprite::GetSpriteByName("dash");
			dash->sprite.shader.id         = 3;
			dash->sprite.frameMilliseconds = dash->lifeTimeLength/4;
			dash->scale                    = glm::vec3(250.0f, 25.0f, 1.0f);
			dash->position                 = glm::vec3(this->position.x + 125.0f * direction,
			                                           this->position.y,
			                                           -1.0f);
			dash->objectType = OBJECT_TYPE_SOLID;
			dash->objectName = OBJECT_NAME_DASH;


			Engine->objects.push_back(dash);
		}	
	}
	
	// std::cout << elapsedMilliseconds << std::endl;
	velocity += acceleration * (elapsedMilliseconds/1000.0f);
	velocity *= 0.9f;

	sprite.direction = (velocity.x >= 0) ? SPRITE_DIRECTION_RIGHT : SPRITE_DIRECTION_LEFT;

}


void Player::Render(glm::mat4 *p, glm::mat4 *v) {
		GameObject::Render(p, v);
}


void Player::CollisionWith(GameObject *collisionTarget) { 
	switch(collisionTarget->objectName) {
		case OBJECT_NAME_BLOCK:
		{
			glm::vec3 v = SAT_AABBUncollisionVector(GetRect(), collisionTarget->GetRect());
			position += v;
			if(fabs(v.y) > fabs(v.x)) {
				onGround = true;
				// velocity.y = 0.0f;
			}
			else {
				// velocity.x = 0.0f;
			}
			break;
		}
		case OBJECT_NAME_ENEMY_FLOAT:
		{
			std::cout << "SHould take damage: " << shouldTakeDamage << std::endl;
			if(shouldTakeDamage){
				position = glm::vec3(500.0f, 500.0f, 0.0f);	
			}
			break;
		}
		default:
		{
			break;
		}
	}
}
