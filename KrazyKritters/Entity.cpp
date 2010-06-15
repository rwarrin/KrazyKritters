#include "Entity.h"

Entity::Entity() {
    x_ = 0;
    y_ = 0;
    dx_ = 0;
    dy_ = 0;

    angle_ = 0;

    living_ = false;
}

Entity::Entity(HGE * hge, HTEXTURE texture, float topSpeed) {
    x_ = 0;
    y_ = 0;
    dx_ = 0;
    dy_ = 0;

	hge_ = hge;

    angle_ = 0;

    living_ = false;

	texture_ = texture;
	texturewidth_ = hge_->Texture_GetWidth(texture_);
	textureheight_ = hge_->Texture_GetHeight(texture_);

	sprite_ = new hgeSprite(texture, 0, 0, texturewidth_, textureheight_);
	sprite_->SetHotSpot(texturewidth_ / 2, textureheight_ / 2);

	topSpeed_ = topSpeed;

	score_ = 0;
}

Entity::~Entity() {
	
}


void Entity::SetEnemyVector(std::vector<Entity *> * vector) {
	enemyVector_ = vector;
}


void Entity::Update() {
	// Does nothing, should never be called
};

bool Entity::GetLivingStatus() {
    return living_;
}

float Entity::GetX() {
    return x_;
}

float Entity::GetY() {
    return y_;
}

float Entity::GetDX() {
    return dx_;
}

float Entity::GetDY() {
    return dy_;
}

float Entity::GetAngle() {
    return angle_;
}

void Entity::SetX(float x) {
    x_ = x;
}

void Entity::SetY(float y) {
    y_ = y;
}

void Entity::SetDX(float dx) {
    dx_ = dx;
}

void Entity::SetDY(float dy) {
    dy_ = dy;
}

void Entity::SetAngle(float angle) {
    angle_ = angle;
}

void Entity::SetLivingStatus(bool living) {
	living_ = living;
}

void Entity::DealDamageToEntity(float damage) {
	hp_ = hp_ - damage;
}

hgeSprite * Entity::GetSprite() {
	return sprite_;
}

AITYPE Entity::GetAIType() {
	return aitype_;
}

void Entity::SetAIType(AITYPE aitype) {
	aitype_ = aitype;
}

int Entity::GetScore() {
	return score_;
}

void Entity::AddToScore(int value) {
	score_ = score_ + value;
}

void Entity::SetHP(float hp) {
	hp_ = hp;
}

float Entity::GetHP() {
	return hp_;
}

int Entity::GetTextureWidth() {
	return texturewidth_;
}

int Entity::GetTextureHeight() {
	return textureheight_;
}