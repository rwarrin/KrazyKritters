#include "Level.h"

Level::Level(HGE * hge, Entity * player) {
	hge_ = hge;
	playerentity_ = player;
	levelcomplete_ = false;
}

Level::Level(HGE * hge, Entity * player, HTEXTURE background, HTEXTURE enemytexture, int numberofenemies) {
	hge_ = hge;
	playerentity_ = player;
	backgroundtexture_ = background;
	enemytexture_ = enemytexture;
	numberofenemies_ = numberofenemies;
	numberofenemiestokill_ = numberofenemies;
	levelcomplete_ = false;
	backgroundsprite_ = new hgeSprite(backgroundtexture_, 0, 0, 800, 600);
}

Level::Level(HGE * hge, Entity * player, HTEXTURE background) {
	hge_ = hge;
	playerentity_ = player;
	backgroundtexture_ = background;
	numberofenemies_ = 0;
	numberofenemiestokill_ = 0;
	levelcomplete_ = false;
	backgroundsprite_ = new hgeSprite(backgroundtexture_, 0, 0, 800, 600);
}

Level::~Level() {
	playerentity_ = 0;
}

void Level::SetPlayerEntity(Entity * player) {
	playerentity_ = player;
}

void Level::SetBackgroundTexture(HTEXTURE background) {
	backgroundtexture_ = background;
}

void Level::SetEnemyTexture(HTEXTURE enemytexture) {
	enemytexture_ = enemytexture;
}

hgeSprite * Level::GetBackgroundSprite() {
	return backgroundsprite_;
}

void Level::SetLevelCompleteStatus(bool complete) {
	levelcomplete_ = complete;
}

bool Level::GetLevelCompleteStatus() {
	return levelcomplete_;
}

std::vector<Entity *> * Level::GetEnemyVector() {
	return &enemyvector_;
}

void Level::AddEnemyToLevel(ENEMYINFO &ei, int numberofenemies) {
	for(int i = 0; i < numberofenemies; i++) {
		enemyvector_.push_back(new Enemy(ei));
	}
	numberofenemies_ = numberofenemies_ + numberofenemies;
}

void Level::InitializeLevel() {
	std::vector<Entity *>::iterator iter;
	for(iter = enemyvector_.begin(); iter != enemyvector_.end(); iter++) {
		Entity * temp = *iter;
		if(temp->GetAIType() == WANDERER || temp->GetAIType() == BOSS)
			numberofenemiestokill_++;
	}
}

void Level::Update() {
	/// TODO:  This function will check on every update whether or not
	// the level is complete.
	int deadcount = 0;

	std::vector<Entity *>::iterator iter;
	for(iter = enemyvector_.begin(); iter != enemyvector_.end(); iter++) {
		Entity * temp = *iter;

		if(temp->GetAIType() == WANDERER || temp->GetAIType() == BOSS) {
			if(temp->GetLivingStatus() == false)
				deadcount++;
		}

	}

	if(deadcount >= numberofenemiestokill_)
		levelcomplete_ = true;

	if(playerentity_->GetHP() <= 0)
		levelcomplete_ = true;
}