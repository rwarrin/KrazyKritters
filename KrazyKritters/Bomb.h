#ifndef BOMB_H
#define BOMB_H

#include "Entity.h"

class Bomb : public Entity {
public:
	Bomb();
	Bomb(HGE * hge, HTEXTURE texture, float starttime, float lifetime);
	Bomb(HGE * hge, HTEXTURE texture, float starttime, float lifetime, float x, float y, float angle);
	virtual ~Bomb();

	virtual void Update();
private:
	float currenttime_;
	float starttime_;
	float lifetime_;
};


#endif  // BOMB_H