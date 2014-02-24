#pragma once
#include "Box.h"
#include "GameObject.h"
#include "constants.h"
#include "d3dUtil.h"
#include "input.h"
#include <math.h>
#include <vector>
using std::vector;

class Player
{
public:
	Player();
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
	void linkInput(Input* in);
	void normalizeDirection();

private:
	Box* box;
	Vector3 position;
	Vector3 direction;
	float speed;
	bool active;
	Matrix world;
	float scale;
	float radius;
	float radiusSquared;
	float height;
	float width;

	//Daniel Ecker
	Input* input;
	int lane, oldLane;
	float laneFudgeFactor;
	float movingTo, distanceToNewPos;
	
	vector<float> lanePositions;

	ID3D10EffectTechnique* mTech;

};

