#pragma once
#include "ofMain.h"

#include "Entity.h"

class Asteroid : public Entity
{
	public:
		Asteroid();
		Asteroid(Asteroid* asteroid);
		~Asteroid();
	
		virtual bool setup();
		bool setup(vector<ofPoint> & shape, float size, float speed, float rotation, ofPoint position, ofPoint direction);
		virtual void update(float deltaTime);
		virtual void draw(bool debug);
	
		bool canBeSplit();

		//new methods
		void split();
		vector<ofPoint> getShape();

	private:
		bool canSplit;
	
		vector<ofPoint> asteroidShape;
};