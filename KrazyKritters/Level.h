#ifndef LEVEL_H
#define LEVEL_H

#include "Entity.h"
#include "Player.h"
#include "Enemy.h"

#include <hge.h>
#include <hgesprite.h>
#include <vector>

class Level {
public:
	Level(HGE * hge, Entity * player);
	Level(HGE * hge, Entity * player, HTEXTURE background, HTEXTURE enemytexture, int numberofenemies);
	Level(HGE * hge, Entity * player, HTEXTURE background);
	~Level();

	void SetBackgroundTexture(HTEXTURE background);
	void SetEnemyTexture(HTEXTURE enemytexture);
	void SetPlayerEntity(Entity * player);

	hgeSprite * GetBackgroundSprite();

	int GetLevelCompleteByEvent();
	bool GetLevelCompleteStatus();
	void SetLevelCompleteStatus(bool complete);

	std::vector<Entity *> * GetEnemyVector();

	void AddEnemyToLevel(ENEMYINFO &ei, int numberofenemies);

	void Update();

	void InitializeLevel();

private:
	HGE * hge_;

	HTEXTURE backgroundtexture_;
	HTEXTURE enemytexture_;

	hgeSprite * backgroundsprite_;

	std::vector<Entity *> enemyvector_;

	Entity * playerentity_;

	bool levelcomplete_;

	int numberofenemies_;
	int numberofenemiestokill_;

	int levelcompleteevent_;

	void MakeBackgroundSprite();
};

#endif  // LEVEL_H