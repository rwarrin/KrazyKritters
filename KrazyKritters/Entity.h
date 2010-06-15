#ifndef ENTITY_H
#define ENTITY_H

#include <hge.h>
#include <hgesprite.h>
#include <math.h>
#include <vector>

enum AITYPE {
	WANDERER = 0,
	PREDATOR,
	BOSS
};

const float FRICTION = 0.96;

class Entity {
public:
    Entity();
	Entity(HGE * hge, HTEXTURE texturename, float topSpeed);
    virtual ~Entity();

    virtual void Update();

    bool GetLivingStatus();

    float GetX();
    float GetY();
    float GetDX();
    float GetDY();
    float GetAngle();
	float GetHP();
	int GetScore();
	int GetTextureWidth();
	int GetTextureHeight();

	AITYPE GetAIType();

    void SetX(float x);
    void SetY(float y);
    void SetDX(float dx);
    void SetDY(float dy);
    void SetAngle(float angle);
    void SetLivingStatus(bool living);
	void SetTexture(HTEXTURE texture);
	void SetAIType(AITYPE aitype);
	void SetHP(float hp);
	void AddToScore(int value);

	void SetEnemyVector(std::vector<Entity *> * vector);

	void DealDamageToEntity(float damage);

	hgeSprite * GetSprite();

protected:
    float x_;
    float y_;
    float dx_;
    float dy_;

    float angle_;

	float topSpeed_;

	float hp_;

    bool living_;

	int score_;

	int texturewidth_;
	int textureheight_;

	HGE * hge_;

	HTEXTURE texture_;
	hgeSprite * sprite_;

	std::vector<Entity *> * enemyVector_;

	AITYPE aitype_;
};

#endif  // ENTITY_H
