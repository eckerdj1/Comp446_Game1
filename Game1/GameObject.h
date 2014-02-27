
#ifndef GameObject_H
#define GameObject_H

//#include "d3dUtil.h"
#include "d3dUtil.h"
#include "Box.h"
#include "constants.h"
#include <math.h>
#include <vector>
using std::vector;

class GameObject
{
public:

	GameObject();
	~GameObject();

	void init(Box *b, float r, Vector3 pos, Vector3 vel, float spd, Vector3 scale);
	void draw();
	void update(float dt);

	void setBox(Box* b) {box = b;}
	void setPosition (Vector3 pos) {position = pos;}
	Vector3 getPosition() {return position;}
	void setVelocity (Vector3 vel) {velocity = vel;}
	void addVelocity(Vector3 vel) {velocity += vel;}
	Vector3 getVelocity() {return velocity;}
	void setSpeed(float s) {speed = s;}
	float getSpeed() {return speed;}
	void setRadius(float r) {radius = r; radiusSquared = (size.x)*(size.x*r);}
	float getRadiusSquare() {return radiusSquared;}
	float getRadius() {return radius;}
	Matrix getWorldMatrix() {return world;}
	void setSize(Vector3 s) {size = s; radiusSquared = (s.x*radius)*(s.x*radius);}
	Vector3 getSize() {return size;}
	void setActive() {active = true;}
	void setInActive() {active = false;}
	bool isActive() {return active;}
	bool isNotActive() {return !active;}
	void setMTech(ID3D10EffectTechnique* m){ mTech = m;}
	bool collided(GameObject *gameObject);
	bool willCollide(GameObject *gameObject, float dt);
	bool onTopOf(GameObject *gameObject);
	bool isAbove(GameObject *gameObject);
	void normlizeVelocity();
	float getBoxBottom();
	void deleteBox();
	bool contains(Vector3);
	DXColor colorAtPoint(float zPos);
	float xRadius();
	float yRadius();
	float zRadius();
	Vector3 cornerAt(int i);

protected:
		Box *box;

private:
	Vector3 position;
	Vector3 velocity;
	Vector3 size;
	vector<Vector3> corners;
	float speed;
	float radius;
	float radiusSquared;
	bool active;
	Matrix world;
	ID3D10EffectTechnique* mTech;
};


#endif
