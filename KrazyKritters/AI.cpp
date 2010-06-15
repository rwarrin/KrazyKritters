/*  
 *  This file contains all of the AI code for the Enemy class.
 *
 *  To add a function to this file first declare it in the Enemy.h file where it says to
 *  declare new functions.
 *
 *  If you need to add variables you can add the to the private section of the enemy class in Enemy.h
 */ 

#include "Enemy.h"

void Enemy::MoveStraight(float speed) {
	x_ += cos(angle_ - M_PI_2) * speed;
	y_ += sin(angle_ - M_PI_2) * speed;
}
    
void Enemy::MoveWave(int waveness, int speed, int topSpeed, int turn){
	// turn: 0 = turn left, 1 = straight, 2 = turn right       
	double temp = static_cast<float>(counter_) / static_cast<float>(waveness);

	if(oscillator_)
		angle_ += 0.01 * (-4 * pow(temp - 0.5, 2) + 1);
	else
		angle_ += -0.01 * (-4 * pow(temp - 0.5, 2) + 1);

	counter_++;

	if(counter_ >= waveness) {
		if (oscillator_)
			oscillator_ = false;
		else
			oscillator_ = true;

		counter_ = 0;
	}
	
	MoveStraight(speed);
}

void Enemy::AI() {
	switch(aitype_) {
	case WANDERER:  // Wanderer AI
		if(ParameterCheck())
			MoveWave(400, topSpeed_, ParameterTurn() * 0.1, 1);
		else
			MoveWave(300, topSpeed_, 10, 30);
		break;
	case BOSS:
	case PREDATOR:  // Predator AI
		distance_ = sqrt(pow(theplayer->GetX() - x_, 2) + pow(theplayer->GetY() - y_, 2));

		if(distance_ <= 200) {
			focusx_ = theplayer->GetX() - (cos(theplayer->GetAngle() - M_PI_2) * sneaky_);
			focusy_ = theplayer->GetY() - (sin(theplayer->GetAngle() - M_PI_2) * sneaky_);
			focusangle_ = atan2(focusy_ - y_, focusx_ - x_);
			
			if((distance_ <= chargedistance_) && (focusangle_ - angle_) <= 0.4)
				MoveStraight(chargespeed_);
			else {
				angle_ = angle_ + (focusangle_ + M_PI_2 - angle_) / agility_;
				MoveStraight(speed_);
			}
		}
		else {
			if(ParameterCheck())
				MoveWave(400, topSpeed_, ParameterTurn() * 0.1, 1);
			else
				MoveWave(300, topSpeed_, ParameterTurn() * 0.1, 30);
		}
	}

	DetectCollisions();
}

int Enemy::ParameterTurn(){
	// 0=left, 1=right
	bool left, top;
	if (x_ < 400)
		left = true;
	else
		left = false;
	if (y_ < 300)
		top = true;
	else
		top = false;

	if (y_ < 50) {
		if (left)
			return 1;
		else
			return -1;
	}

	if (y_ > 750) {
		if (left)
			return -1;
		else
			return 1;
	}

	if (x_ < 50) {
		if (top)
			return -1;
		else
			return 1;
	}

	if (x_ > 550) {
		if (top)
			return 1;
		else
			return -1;
	}
}

int Enemy::ParameterCheck() {
	if ((x_ < 75) || (x_ > 725) || (y_ < 75 ) || (y_ > 525))
		return 1;

	return 0;
}

void Enemy::DetectCollisions() {
	// Detect if the enemy is moving off of the screen and reposition it
	// at the other side
	//x_ += dx_; 
	//y_ += dy_;
	
	if(x_ > 832) {
		x_ = -31;
	}
	if(x_ < -32) {
		x_ = 831;
	}
	if(y_ > 632) {
		y_ = -31;
	}
	if(y_ < -32) {
		y_ = 631;
	}
}
