#include "Player.h"

Player::Player()
{	
	color = BLACK;
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

void Player::init(Box *b, float r, Vector3 pos, Vector3 dir, float sp, float s)
{
	box = b;
	radius = r;
	radius *= 1.01; //fudge factor
	position = pos;
	direction = dir;
	speed = sp;
	scale = s;
	radiusSquared = radius * radius;
}

void Player::update(float dt)
{
	float changeBy = 0.02f;
	///Red value up/down
	if(input->isKeyDown(PlayerRUpKey)) {
		color.r  += changeBy;
		if (color.r > 1)
			color.r = 1.0f;
		if (color.r < 0)
			color.r = 0.0f;
	}
	if(input->isKeyDown(PlayerRDownKey)) {
		color.r  -= changeBy;
		if (color.r > 1)
			color.r = 1.0f;
		if (color.r < 0)
			color.r = 0.0f;
	}
	///Green value up/down
	if(input->isKeyDown(PlayerGUpKey)) {
		color.g  += changeBy;
		if (color.g > 1)
			color.g = 1.0f;
		if (color.g < 0)
			color.g = 0.0f;
	}
	if(input->isKeyDown(PlayerGDownKey)) {
		color.g  -= changeBy;
		if (color.g > 1)
			color.g = 1.0f;
		if (color.g < 0)
			color.g = 0.0f;
	}
	///Blue value up/down
	if(input->isKeyDown(PlayerBUpKey)) {
		color.b  += changeBy;
		if (color.b > 1)
			color.b = 1.0f;
		if (color.b < 0)
			color.b = 0.0f;
	}
	if(input->isKeyDown(PlayerBDownKey)) {
		color.b  -= changeBy;
		if (color.b > 1)
			color.b = 1.0f;
		if (color.b < 0)
			color.b = 0.0f;
	}
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

void Player::move() {

	Vector3 dir = Vector3(0,0,0);

	if (input->wasKeyPressed(PlayerLeftKey)) {
		lane--;
	}
	if (input->wasKeyPressed(PlayerRightKey)) {
		lane++;
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

void Player::linkInput(Input* in)
{
	input = in;
}

void Player::normalizeDirection()
{
	float length = sqrt(D3DXVec3LengthSq(&direction));
	direction = direction / length;
}