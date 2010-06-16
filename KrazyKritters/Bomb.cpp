#include "Bomb.h"

Bomb::Bomb(HGE * hge, HTEXTURE texture, float starttime, float lifetime) : Entity(hge, texture, 0.0) {
	// Do basic bomb initialization here, this function really shouldn't be used
	// but is included for completeness
	starttime_ = starttime;
	lifetime_ = lifetime;
	x_ = 100.0f;
	y_ = 100.0f;
	dx_ = 0.0f;
	dy_ = 0.0f;
	angle_ = 0.0f;
	living_ = true;
	hp_ = 1.0f;
}

Bomb::Bomb(HGE *hge, HTEXTURE texture, float starttime, float lifetime, float x, float y, float angle) : Entity(hge, texture, 0.0) {
	starttime_ = starttime;
	lifetime_ = lifetime;
	x_ = x;
	y_ = y;
	dx_ = 0.0;
	dy_ = 0.0;
	angle_ = angle;
	living_ = true;
	hp_ = 1.0f;
}

Bomb::~Bomb() {

}

void Bomb::Update() {
	currenttime_ = hge_->Timer_GetTime();

	if(currenttime_ - starttime_ >= lifetime_) {
		living_ = false;
	}

	// Detect collisions with enemies here
	// Be sure to set use the SetEnemyVector() function before calling
	// this Update() function.
	std::vector<Entity *>::iterator iter;
	for(iter = enemyVector_->begin(); iter != enemyVector_->end(); iter++) {
		Entity * temp = *iter;

		if(temp->GetLivingStatus() == true) {
			float enemyX = temp->GetX();
			float enemyY = temp->GetY();

			float distance = sqrt(pow((enemyX - x_), 2) + pow((enemyY - y_), 2));

			if(temp->GetAIType() == WANDERER) {
				if(distance <= temp->GetTextureWidth() / 1.0)  {
					temp->DealDamageToEntity(15);
					living_ = false;
				}
			}

			if(temp->GetAIType() == PREDATOR) {
				if(distance <= temp->GetTextureWidth() / 1.5) {
					temp->DealDamageToEntity(1000);
					living_ = false;
				}
			}

			if(temp->GetAIType() == BOSS) {
				if(distance <= temp->GetTextureWidth() / 1.7) {
					temp->DealDamageToEntity(1000);
					living_ = false;
				}
			}
		}
	}

}