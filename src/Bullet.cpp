#include "Bullet.h"

Bullet::Bullet()
{
	position.x = 0;
	position.y = 0;
	
	direction = ofPoint(0, 0);

	rotation = 0;
}

Bullet::~Bullet(void)
{

}

bool Bullet::setup()
{
	return false;
}

bool Bullet::setup(ofPoint shipPosition, ofPoint shipDirection, float size, float speed, float lifeTime, string origin)
{
	position.x = shipPosition.x;
	position.y = shipPosition.y;

	direction.x = -shipDirection.x;
	direction.y = -shipDirection.y;
	
	tempStartPosition = shipPosition;

	this->size = size;
	this->speed = speed;
	this->lifeTime = lifeTime;
	this->origin = origin;

	return true;
}

void Bullet::update(float deltaTime)
{
	position += direction * speed * deltaTime;
	lifeTime -= 0.5;
	marginsWrap();
}

void Bullet::draw(bool debug)
{	
	ofPushStyle();
	ofPushMatrix();
		ofRect(position.x, position.y, size, size);
	ofPopMatrix();
	ofPopStyle();
}

void Bullet::kill()
{
	lifeTime = 0;
}

bool Bullet::getMaxFireDistanceReached()
{
	if ( lifeTime <= 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

float Bullet::getDistanceBetweenShipAndBullet()
{
	return ofDist(tempStartPosition.x,tempStartPosition.y, position.x, position.y);
}
