#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"

class Player : public Entity {
public:
    Player();
	Player(HTEXTURE texture, HGE * hge);
	Player(HTEXTURE texture, HGE * hge, float x, float y, float dx, float dy, float angle, float topSpeed);
    virtual ~Player();

    virtual void Update();

	void SetHGEInterface(HGE * hge);

private:
	float mouseX_;
	float mouseY_;
	float temporaryimmunity_;
};

#endif
