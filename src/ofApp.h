#pragma once

#include "ofMain.h"

#include "ofxXmlSettings.h"

#include "Asteroid.h"
#include "Bullet.h"
#include "SpaceShip.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

	private:
		void loadAsteroids();

		void generateAsteroids(int numAsteroids);

		void normalizeOfPoint(ofPoint& point);

		ofxXmlSettings XML;

		vector<vector<ofPoint>> asteroidsDefinitions;

		vector<Asteroid*> asteroids;

		bool debug;

		//components
		SpaceShip* player1;
		SpaceShip* player2;
		vector<Bullet*> bullets;


		//event methods
		void spaceShipFiresEvent(ofSpaceShipFireEventArgs &e);
		void spaceShipFiresEvent2(ofSpaceShipFireEventArgs &e);

		//new methods
		void addAsteroid(vector<Asteroid*> &vec, vector<Asteroid*>::iterator &it, Asteroid* asteroid);
		void eraseBullet(vector<Bullet*> &vec, vector<Bullet*>::iterator &it);
};
