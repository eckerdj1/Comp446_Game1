#include "Player.h"

Player::Player()
{	
	wheelVal = 0.0f;
	color.r = 1.0f;
	color.g = 0.0f;
	color.b = 0.0f;

	elapsed = 0.0f;
	hoverTimer = 0.0f;
	keyWaitTime = 0.127f;

	speed = 0;
	active = true;
	Identity(&world);
	//Changes by: Daniel J. Ecker
	input = 0;
	lane = 2;
	oldLane = 2;
	laneFudgeFactor = 0.005f;
	distanceToNewPos = 0.0f;
	for (int i=0; i<5; ++i)
	{
		float l = -12.0f;
		l += 6.0f * i;
		lanePositions.push_back(l);
	}
	movingTo = lanePositions[2];
}


Player::~Player(void)
{
	//delete box;
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

void Player::init(Box *b, float r, Vector3 pos, Vector3 dir, float sp, Vector3 sz, Audio* audio)
{
	box = b;
	radius = r;
	radius *= 1.01; //fudge factor
	position = pos;
	direction = dir;
	speed = sp;
	size = sz;
	radiusSquared = radius * radius;
	Player::audio = audio;
}

void Player::update(float dt)
{
	float changeBy = 0.003f;
	if(input->isKeyDown(0x4A) || keyPressed(PlayerColorUpKey)) {
		colorShiftDown(changeBy);
	}
	if(input->isKeyDown(0x4C) || keyPressed(PlayerColorDownKey)) {
		colorShiftUp(changeBy);
	}

	hoverTimer += dt * 2.7f;

	float hoverHeight =  0.45f * cos(hoverTimer) +  2.0f;
	position.y = hoverHeight;


	box->releaseVBuffer();
	box->setVertexColor(color, color);
	
	if (!active)
		return;
	if (fabs(position.x - movingTo) > laneFudgeFactor)
	{
		direction.x = movingTo - position.x;
		normalizeDirection();
		distanceToNewPos = fabs(position.x - movingTo);
	}
	else
	{
		oldLane = lane;
	}

	speed = distanceToNewPos * 15.0f;// 3.0f;
	if (speed < 1)
		speed = 1.0f;
	
	position += direction * speed * dt;
	Identity(&world);
	Translate(&world, position.x, position.y, position.z);

}

void Player::move(float dt) {

	Vector3 dir = Vector3(0,0,0);

	elapsed += dt;

	if (keyPressed(PlayerLeftKey) && elapsed > keyWaitTime) {
		lane--;
		elapsed = 0.0f;
		if (lane >= 0) {
			audio->playCue(WOOSH);
		}
	}
	if (keyPressed(PlayerRightKey) && elapsed > keyWaitTime) {
		lane++;
		elapsed = 0.0f;
		if (lane <= 4) {
			audio->playCue(WOOSH);
		}
	}
	if (lane < 0)
		lane = 0;
	if (lane > 4)
		lane = 4;
	//else if (GetAsyncKeyState('W') & 0x8000) {
	//	//dir.y = 1;
	//	dir.z = 1;
	//}
	//else if (GetAsyncKeyState('S') & 0x8000) {
	//	//dir.y = -1;
	//	dir.z = -1;
	//}

	movingTo = lanePositions[lane];

	return;

}

bool Player::isWithin(float range, GameObject* g)
{
	float distSquared = D3DXVec3LengthSq(&(position - g->getPosition()));
	float rSquared = (float)pow((double)range, 2.0);
	if (distSquared < rSquared)
		return true;
	return false;
}

bool Player::contains(Vector3 point)
{
	if (point.z >= position.z - size.z && point.z <= position.z + size.z)
	{
		if (point.y >= position.y - size.y && point.y <= position.y + size.y)
		{
			if (point.x >= position.x - size.x && point.x <= position.x + size.x)
			{
				return true;
			}
		}
	}
	return false;
}

bool Player::collided(GameObject *gameObject)
{
	GameObject* g = gameObject;
	for (int i=0; i<8; ++i)
	{
		if (contains(g->getPosition() + g->cornerAt(i)))
		{
			return true;
		}
	}

	return false;
}

void Player::linkInput(Input* in)
{
	input = in;
}

void Player::normalizeDirection()
{
	float length = sqrt(D3DXVec3LengthSq(&direction));
	direction = direction / length;
}

//Andy Miller
void Player::colorShiftUp(float x) {
	wheelVal += x;
	if(wheelVal > 6.0f)
		wheelVal -= 6.0f;

	if(wheelVal > 0.0f && wheelVal < 1.0f) {
		color.r = 1.0f;
		color.g = wheelVal;
		color.b = 0.0f;
	}
	if(wheelVal > 1.0f && wheelVal < 2.0f) {
		color.r = 2.0f - wheelVal;
		color.g = 1.0f;
		color.b = 0.0f;
	}
	if(wheelVal > 2.0f && wheelVal < 3.0f) {
		color.r = 0.0f;
		color.g = 1.0f;
		color.b = wheelVal - 2.0f;
	}
	if(wheelVal > 3.0f && wheelVal < 4.0f) {
		color.r = 0.0f;
		color.g = 4.0f - wheelVal;
		color.b = 1.0f;
	}
	if(wheelVal > 4.0f && wheelVal < 5.0f) {
		color.r = wheelVal - 4.0f;
		color.g = 0.0f;
		color.b = 1.0f;
	}
	if(wheelVal > 5.0f && wheelVal < 6.0f) {
		color.r = 1.0f;
		color.g = 0.0f;
		color.b = 6.0f - wheelVal;
	}
}

void Player::colorShiftDown(float x) {
	wheelVal -= x;
	if(wheelVal < 0.0f)
		wheelVal += 6.0f;

	if(wheelVal > 0.0f && wheelVal < 1.0f) {
		color.r = 1.0f;
		color.g = wheelVal;
		color.b = 0.0f;
	}
	if(wheelVal > 1.0f && wheelVal < 2.0f) {
		color.r = 2.0f - wheelVal;
		color.g = 1.0f;
		color.b = 0.0f;
	}
	if(wheelVal > 2.0f && wheelVal < 3.0f) {
		color.r = 0.0f;
		color.g = 1.0f;
		color.b = wheelVal - 2.0f;
	}
	if(wheelVal > 3.0f && wheelVal < 4.0f) {
		color.r = 0.0f;
		color.g = 4.0f - wheelVal;
		color.b = 1.0f;
	}
	if(wheelVal > 4.0f && wheelVal < 5.0f) {
		color.r = wheelVal - 4.0f;
		color.g = 0.0f;
		color.b = 1.0f;
	}
	if(wheelVal > 5.0f && wheelVal < 6.0f) {
		color.r = 1.0f;
		color.g = 0.0f;
		color.b = 6.0f - wheelVal;
	}
}

float Player::xRadius()
{
	return size.x / 2.0f;
}
float Player::yRadius()
{
	return size.y / 2.0f;
}
float Player::zRadius()
{
	return size.z / 2.0f;
}