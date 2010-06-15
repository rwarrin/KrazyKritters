#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"

struct ENEMYINFO {
	HGE * hge;
	Entity * player;
	HTEXTURE texture;
	float topspeed;
	float hp;
	AITYPE aitype;
};

class Enemy : public Entity {
public:
    Enemy();
    Enemy(HGE * hge, HTEXTURE texture, Entity * player);
    Enemy(HGE * hge, HTEXTURE texture, Entity * player, float x, float y, float dx, float dy, float angle, float topSpeed, AITYPE aitype);
	Enemy(ENEMYINFO &ei);
    virtual ~Enemy();

    virtual void Update();

    void SetPlayerObject(Entity * player);

private:
	void AI();
	void DetectCollisions();

	void MoveStraight(float speed);
	void MoveWave(int waveness, int speed, int topSpeed, int turn);
	int ParameterCheck();
	int ParameterTurn();

    Entity * theplayer;

	// General AI variables
	float distance_;

	// Wanderer AI variables
	int counter_;
	bool oscillator_;

	// Predator AI variables
	float focusx_;
	float focusy_;
	float focusangle_;
	float speed_;
	float agility_;
	float sneaky_;
	float chargedistance_;
	float chargespeed_;
};

#endif  // ENEMY_H
