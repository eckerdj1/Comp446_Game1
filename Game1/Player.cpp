#include "Player.h"


Player::Player(void)
{
	speed = 0;
	active = true;
	Identity(&world);
}


Player::~Player(void)
{
	delete box;
}

void Player::draw()
{
	if (!active)
		return;
    D3D10_TECHNIQUE_DESC techDesc;
    mTech->GetDesc( &techDesc );
    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
        mTech->GetPassByIndex( p )->Apply(0);
        box->draw();
    }
		/*box->draw();*/
}

void Player::init(Box *b, float r, Vector3 pos, Vector3 vel, float sp, float s)
{
	box = b;
	radius = r;
	radius *= 1.01; //fudge factor
	position = pos;
	velocity = vel;
	speed = sp;
	scale = s;
	radiusSquared = radius * radius;
}

void Player::update(float dt)
{
	if (!active)
		return;
	position += velocity * speed * dt;
	Identity(&world);
	Translate(&world, position.x, position.y, position.z);

}

void Player::move() {

	Vector3 dir = Vector3(0,0,0);

	if (GetAsyncKeyState('A') & 0x8000) {
		dir.x = -1;
	}
	else if (GetAsyncKeyState('D') & 0x8000) {
		dir.x = 1;
	}
	else if (GetAsyncKeyState('W') & 0x8000) {
		//dir.y = 1;
		dir.z = 1;
	}
	else if (GetAsyncKeyState('S') & 0x8000) {
		//dir.y = -1;
		dir.z = -1;
	}

	Normalize(&dir, &dir);

	dir *= speed;

	velocity = dir;

	return;

}

bool Player::collided(GameObject *gameObject)
{
	if (!active || gameObject->isNotActive())
		return false;
	Vector3 diff = position - gameObject->getPosition();
	float length = D3DXVec3LengthSq(&diff);
	float radii = radiusSquared + gameObject->getRadiusSquare();
	if (length <= radii)
		return true;
	return false;
}