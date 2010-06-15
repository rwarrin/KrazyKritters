#include "Enemy.h"

Enemy::Enemy(HGE * hge, HTEXTURE texture, Entity * player) : Entity(hge, texture, 2.0) {
    theplayer = player;
    x_ = 10;
    y_ = 10;
    dx_ = 1;
    dy_ = 1;
    angle_ = 1;
    living_ = true;
	counter_ = rand() % 5;
	oscillator_ =  rand() % 1;
	aitype_ = WANDERER;
	speed_ = 1;
	agility_ = 1;
	sneaky_ = 1;
	chargedistance_ = 150;
	chargespeed_ = 3;
}

Enemy::Enemy(HGE * hge, HTEXTURE texture, Entity * player, float x, float y, float dx, float dy, float angle, float topSpeed, AITYPE aitype) : Entity(hge, texture, topSpeed) {
    theplayer = player;
    x_ = x;
    y_ = y;
    dx_ = dx;
    dy_ = dy;
    angle_ = angle;
	living_ = true;
	counter_ = rand() % 5;
	oscillator_ =  rand() % 1;
	aitype_ = aitype;
	speed_ = 1;
	agility_ = 1;
	sneaky_ = 1;
	chargedistance_ = 150;
	chargespeed_ = 3;
}

Enemy::Enemy(ENEMYINFO &ei) : Entity(ei.hge, ei.texture, ei.topspeed) {
	theplayer = ei.player;
	x_ = rand() % 800;
	y_ = rand() % 600;
	dx_ = 0.0f;
	dy_ = 0.0f;
	angle_ = rand() % 360;
	living_ = true;
	hp_ = ei.hp;
	aitype_ = ei.aitype;
	counter_ = rand() % 5;
	oscillator_ = rand() % 1;
	speed_ = 1;
	agility_ = 1;
	sneaky_ = 1;
	chargedistance_ = 150;
	chargespeed_ = 3.0;
}

Enemy::~Enemy() {
    theplayer = 0;
}

void Enemy::SetPlayerObject(Entity * player) {
    theplayer = player;
}

void Enemy::Update() {
	if(hp_ <= 0) {
		living_ = false;

		if(aitype_ == WANDERER)
			theplayer->AddToScore(25);
		
		if(aitype_ == PREDATOR)
			theplayer->AddToScore(-10);

		if(aitype_ == BOSS)
			theplayer->AddToScore(100);
	}

	AI();
}