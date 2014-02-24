#pragma once
#include "Box.h"
#include "GameObject.h"
#include "constants.h"
#include "d3dUtil.h"
#include <math.h>

class Player
{
public:
	Player(void);
	~Player(void);

	void init(Box *b, float r, Vector3 pos, Vector3 vel, float sp, float s);
	void update(float dt);
	void draw();
	void move();
	bool collided(GameObject *gameObject);
	Matrix getWorldMatrix() {return world;}
	void setMTech(ID3D10EffectTechnique* m){ mTech = m;}
	void setActive(bool a) {active = a;}
	bool isActive() { return active; }
private:
	Box* box;
	Vector3 position;
	Vector3 velocity;
	float speed;
	bool active;
	Matrix world;
	float scale;
	float radius;
	float radiusSquared;
	float height;
	float width;
	ID3D10EffectTechnique* mTech;

};

