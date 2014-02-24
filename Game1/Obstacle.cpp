#include "Obstacle.h"


Obstacle::Obstacle(void)
{
}


Obstacle::~Obstacle(void)
{
	GameObject::~GameObject();
}

void Obstacle::update(float dt) {

	GameObject::update(dt);

	Vector3 pos = GameObject::getPosition();
	if (pos.z < -200) {
		pos.z = rand()%150;
		pos.z += 200;
		GameObject::setPosition(pos);
	}
}
