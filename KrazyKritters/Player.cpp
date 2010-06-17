#include "Player.h"

Player::Player() {
    x_ = 10;
    y_ = 10;
    dx_ = 1;
    dy_ = 1;
    angle_ = 1;
    living_ = true;
	hp_ = 200.0f;
}

Player::Player(HTEXTURE texture, HGE * hge) : Entity(hge, texture, 2.0) {
	x_ = 10;
	y_ = 10;
	dx_ = 1;
	dy_ = 1;
	angle_ = 1;
	living_ = true;
	hp_ = 200.0f;
}

Player::Player(HTEXTURE texture, HGE * hge, float x, float y, float dx, float dy, float angle, float topSpeed) : Entity(hge, texture, topSpeed) {
	x_ = x;
	y_ = y;
	dx_ = dx;
	dy_ = dy;
	angle_ = angle;
	living_ = true;
	hp_ = 200.0f;
}

Player::~Player() {

}

void Player::Update() {
	float dTime = hge_->Timer_GetDelta();

    // Get Input (This is very basic and will be evolved into better movement code)
	hge_->Input_GetMousePos(&mouseX_, &mouseY_);
	mouseX_ = mouseX_ - x_ - 15;
	mouseY_ = mouseY_ - y_;

	angle_ = atan2(mouseY_, mouseX_) + 1.57;

	if(hge_->Input_GetKeyState(HGEK_W)) {  // Move forwards, toward mouse
		if( (abs(mouseX_) > 7) || (abs(mouseY_) > 7) ) {
			dy_ += sin(angle_ - M_PI_2) * 0.2;
			dx_ += cos(angle_ - M_PI_2) * 0.2;
		}
	}
	
	if(hge_->Input_GetKeyState(HGEK_S)) {  // Move backwards, away from mouse
		dy_ += sin(angle_ - (M_PI + M_PI_2)) * 0.2;
		dx_ += cos(angle_ - (M_PI + M_PI_2)) * 0.2;
	}

	if(hge_->Input_GetKeyState(HGEK_A)) {  // Move Left around mouse
		dy_ += sin(angle_ - (M_PI - M_PI_4)) * 0.2;
		dx_ += cos(angle_ - (M_PI - M_PI_4)) * 0.2;
	}

	if(hge_->Input_GetKeyState(HGEK_D)) {  // Move Right around mouse
		dy_ += sin(angle_ - (M_PI - M_PI_2 - M_PI_4)) * 0.2;
		dx_ += cos(angle_ - (M_PI - M_PI_2 - M_PI_4)) * 0.2;
	}

	double h = sqrt(pow(dx_, 2) + pow(dy_, 2));
	if(h > topSpeed_) {
		double temp = atan2(dy_, dx_);
		dx_ = cos(temp) * topSpeed_;
		dy_ = sin(temp) * topSpeed_;
	}
	
	// Apply friction? who knows.
	dx_ = dx_ * FRICTION;
	dy_ = dy_ * FRICTION;

	// Update the x and y coordinates of the player
	x_ += dx_;
	y_ += dy_;

	// some movement calculations and collision detection	
	x_+=dx_; 
	y_+=dy_;
	
	if(x_ > 784) {
		x_ = 784 - (x_ - 784);
		dx_ = -dx_;
	}
	if(x_ < 16) {
		x_ = 16 + 16 - x_;
		dx_ = -dx_;
	}
	if(y_ > 584) {
		y_ = 584 - (y_ - 584);
		dy_ = -dy_;
	}
	if(y_ < 16) {
		y_ = 16 + 16 - y_;
		dy_ = - dy_;
	}

	/// TODO:  DO COLLISION DETECTION WITH ENEMIES HERE
	std::vector<Entity *>::iterator iter;
	for(iter = enemyVector_->begin(); iter != enemyVector_->end(); iter++) {
		Entity * temp = *iter;

		if(temp->GetLivingStatus() == true) {
			float enemyX = temp->GetX();
			float enemyY = temp->GetY();

			float distance = sqrt( pow((enemyX - x_), 2) + pow((enemyY - y_), 2) );

			if(temp->GetAIType() == WANDERER) {
				if(distance <= temp->GetTextureWidth() / 1.2) //45)
					temp->DealDamageToEntity(5);
			}
			else if(temp->GetAIType() == PREDATOR) {
				if(distance <= temp->GetTextureWidth() / 1.5) {//30) {
					temp->DealDamageToEntity(50);
					if(temporaryimmunity_ <= 0)
						hp_ = hp_ - 10;

						temporaryimmunity_ = 5;
				}
			}
			else if(temp->GetAIType() == PREDATOR_IMMUNE) {
				if(distance <= temp->GetTextureWidth() / 1.5) {
					if(temporaryimmunity_ <= 0)
						hp_ = hp_ - 15;

						temporaryimmunity_ = 5;
				}
            }
			else {  // if(temp->GetAIType() == BOSS)
				if(distance <= temp->GetTextureWidth() / 1.5) {
					if(temporaryimmunity_ <= 0)
						hp_ = hp_ - 20;

					temporaryimmunity_ = 5;
				}
			}
		}
	}  // For
	if(temporaryimmunity_ > 0)
		temporaryimmunity_--;
}

void Player::SetHGEInterface(HGE * hge) {
	hge_ = hge;
}
