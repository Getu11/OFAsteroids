#pragma once
#include "ofMain.h"

#include "Entity.h"

class Bullet : public Entity
{
	public:
		Bullet();
		~Bullet();
	
		virtual bool setup();
		bool setup(ofPoint spaceShipPosition, ofPoint spaceShipDirection, float size, float speed, float lifeTime, string origin);
		virtual void update(float deltaTime);
		virtual void draw(bool debug);
		string origin;
		void kill();

		void checkAndSetIsBreakable();

		void startPositionSetup(ofPoint shipCoord, ofPoint shipDirection,ofPoint shipSize);
		bool getMaxFireDistanceReached();
		float getDistanceBetweenShipAndBullet();
	
	private:
		float fireDistance;
		float lifeTime;
		ofPoint tempStartPosition;
};