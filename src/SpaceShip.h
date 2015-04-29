#pragma once
#include "ofMain.h"

#include "Entity.h"

struct ofSpaceShipFireEventArgs
{
	ofPoint spaceShipPosition;
	ofPoint spaceShitDirection;
};

class SpaceShip : public Entity
{	
	public:
		SpaceShip();
		~SpaceShip();
	
		virtual bool setup();
		virtual void update(float deltaTime);
		virtual void draw(bool debug);

		void resetSpaceShip();

		void addThrust(float thrust);

		void keyPressed(ofKeyEventArgs & args);
		void keyReleased(ofKeyEventArgs & args);

		void setIsBeingDestroyed();
		float getTimeIsDestroyed();
		bool player1, player2;

		int score;

		ofEvent<ofSpaceShipFireEventArgs> spaceShipFires;

	private:
		bool turnLeft, turnRight, thrust, backThrust, isFiring, isDestroyed;
		float timeIsDestroyed;
		int bulletCount;
};