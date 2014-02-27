#pragma once
#include "Box.h"
#include "GameObject.h"
#include "constants.h"
#include "d3dUtil.h"
#include "input.h"
#include <math.h>
#include <vector>
#include "audio.h"
using std::vector;

class Player
{
public:
	Player();
	~Player(void);

	void init(Box *b, float r, Vector3 pos, Vector3 vel, float sp, Vector3 sz, Audio* audio);
	void update(float dt);
	void draw();
	void move(float dt);
	bool isWithin(float range, GameObject* g);
	bool collided(GameObject *gameObject);
	Matrix getWorldMatrix() {return world;}
	void setMTech(ID3D10EffectTechnique* m){ mTech = m;}
	void setActive(bool a) {active = a;}
	bool isActive() { return active; }
	void linkInput(Input* in);
	void normalizeDirection();
	bool contains(Vector3);
	float xRadius();
	float yRadius();
	float zRadius();
	float getWheelVal() {return wheelVal;}
	Vector3 getPosition() {return position;}
	DXColor getColor() {return color;}

private:
	Box* box;
	Audio* audio;
	Vector3 position;
	Vector3 direction;
	Vector3 size;
	float speed;
	bool active;
	Matrix world;
	float scale;
	float radius;
	float radiusSquared;
	float height;
	float width;
	float hoverTimer;

	//Andy Miller
	DXColor color;
	float wheelVal; //between 0.0f and 6.0f
	void colorShiftUp(float x);
	void colorShiftDown(float x);

	
	//Daniel Ecker
	Input* input;
	int lane, oldLane;
	float laneFudgeFactor;
	float movingTo, distanceToNewPos;

	float elapsed, keyWaitTime;
	
	vector<float> lanePositions;

	ID3D10EffectTechnique* mTech;

};

