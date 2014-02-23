
#include "GameObject.h"

GameObject::GameObject()
{
	radius = 0;
	speed = 0;
	active = true;
	Identity(&world);
}

GameObject::~GameObject()
{
	box = NULL;
}

void GameObject::draw()
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

void GameObject::init(Box *b, float r, Vector3 pos, Vector3 vel, float sp, float s)
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

void GameObject::update(float dt)
{
	if (!active)
		return;
	position += velocity * speed * dt;
	Identity(&world);
	Translate(&world, position.x, position.y, position.z);

}

bool GameObject::collided(GameObject *gameObject)
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

float GameObject::getBoxBottom()
{
	return position.y - 1.0f;
}

bool GameObject::willCollide(GameObject *gameObject, float dt)
{
	if (!active || gameObject->isNotActive())
		return false;
	Vector3 diff = (position + velocity * speed * dt) - (gameObject->getPosition() + gameObject->getVelocity() * gameObject->getSpeed() * dt);
	float length = D3DXVec3LengthSq(&diff);
	float radii = radiusSquared + gameObject->getRadiusSquare();
	if (length <= radii)
		return true;
	return false;
}

bool GameObject::onTopOf(GameObject *gameObject)
{
	if (!active || gameObject->isNotActive())
		return false;
	if (isAbove(gameObject))
		if (position.y - gameObject->position.y > 1.95f && position.y - gameObject->position.y < 2.05f)
		{
			return true;
		}
	return false;
}

bool GameObject::isAbove(GameObject* gameObject)
{
	if (!active)// || gameObject->isNotActive())
		return false;
	if (position.y - gameObject->position.y >= 1.95f)
	{
		if (fabs(position.x - gameObject->position.x) <= 0.1f && fabs(position.z - gameObject->position.z) <= 0.1f)
			return true;
		else 
			return false;
	}
	return false;
}

void GameObject::normlizeVelocity()
{
	//float distSquared = velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z;
	//Vector3 vSquared(velocity.x * velocity.x, velocity.y * velocity.y, velocity.z * velocity.z);
	//velocity = vSquared / distSquared;
	if (velocity.x != 0)
		velocity.x /= fabs(velocity.x);
	if (velocity.y != 0)
		velocity.y /= fabs(velocity.y);
	if (velocity.z != 0)
		velocity.z /= fabs(velocity.z);
}