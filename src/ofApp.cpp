#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	// Set framerate to 60 FPS
	ofSetFrameRate(60);

	// Load Asteroids from XML
	loadAsteroids();

	// Create Asteroids
	generateAsteroids(4);

	// General logic
	ofBackground(0); // Set background to black

	// Debug
	debug = false;

	//generate players
	player1 = new SpaceShip();
	player1->setup();
	player1->player1 = true;

	player2 = new SpaceShip();
	player2->setup();
	player2->player2 = true;

	//suscribe player fire event
	ofAddListener(player1->spaceShipFires, this, &ofApp::spaceShipFiresEvent);
	ofAddListener(player2->spaceShipFires, this, &ofApp::spaceShipFiresEvent2);
	//set initial score

}

//--------------------------------------------------------------
void ofApp::update()
{
	// We get the time that last frame lasted, and use it to update asteroids logic
	// so their behaviour is independent to the framerate
		// We get the time that last frame lasted, and use it to update asteroids logic
	// so their behaviour is independent to the framerate
	float elapsedTime = ofGetLastFrameTime();
	
	//update players
	player1->update( elapsedTime );
	player2->update( elapsedTime );
	
	//vector<Asteroid*>::iterator itA = asteroids.begin();
	//for(int i = 0; i < asteroids.size(); i++) 
	for(vector<Asteroid*>::iterator itA = asteroids.begin(); itA != asteroids.end();)
	{
		bool addAst = true;
		(*itA)->update(elapsedTime);
		if((*itA)->getCollision(player1) && player1->getTimeIsDestroyed() < 0)
		{
			player1->setIsBeingDestroyed();
		}



		if((*itA)->getCollision(player2) && player2->getTimeIsDestroyed() < 0)
		{
			player2->setIsBeingDestroyed();
		}
		//check bullets collisions with asteroids
		//for(int i = 0; i < bullets.size(); i++) 
		for(vector<Bullet*>::iterator itB = bullets.begin(); itB != bullets.end();)
		{
			bool addBullet = true;
			if((*itA)->getCollision((*itB)))
			{
				if((*itA)->canBeSplit())
				{
					(*itA)->split();
					Asteroid* asteroid = new Asteroid( (*itA) );
					asteroid->setup();
					addAsteroid(asteroids, itA, asteroid );
					//increase score
					if((*itB)->origin == "player1") player1->score += 50;
					if((*itB)->origin == "player2") player2->score += 50;
					break;
				}
				else
				{
					itA = asteroids.erase(itA);
					addAst = false;
					//increase score
					if((*itB)->origin == "player1") player1->score += 100;
					if((*itB)->origin == "player2") player2->score += 100;
					break;
				}
				itB = bullets.erase(itB);
				addBullet = false;
			}
			if(addBullet)
				itB++;
		}
		if(addAst)
			itA++;
	}

	for(vector<Bullet*>::iterator it = bullets.begin(); it != bullets.end();)
	{
		if((*it)->getMaxFireDistanceReached())
		{
			it = bullets.erase(it);
		}
		else
		{
			(*it)->update(elapsedTime);
			it++;
		}
		
	}
}

//--------------------------------------------------------------
void ofApp::draw()
{
	for(int i = 0; i < asteroids.size(); i++) 
	{
		asteroids[i]->draw(debug);
	}

	for(int i = 0; i < bullets.size(); i++) 
	{
		bullets[i]->draw(debug);
	}

	//draw score
	ofPushStyle();
		ofSetColor(255);
		ofDrawBitmapString("Player 1: " + ofToString(player1->score) + " PTS", ofGetWindowWidth() - 400 , 15);
		ofDrawBitmapString("Player 2: " + ofToString(player2->score) + " PTS", ofGetWindowWidth() - 200 , 15);
	ofPopStyle();

	//draw information
	if(debug)
	{
		ofPushStyle();
			ofSetColor(255);
			//draw FPS
			ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, 10);
			//spaceship information
			ofDrawBitmapString(ofToString( player1->getTimeIsDestroyed() ), 10, 20);
			ofDrawBitmapString(ofToString( player2->getTimeIsDestroyed() ), 10, 20);

		ofPopStyle();
	}

	//draw players
	player1->draw( debug );
	player2->draw( debug );


	if(player1->score > player2->score && asteroids.size() == 0) {
		ofPushStyle();
			ofSetColor(255);
			ofDrawBitmapString("Player 1 wins", ofGetWindowWidth()/2,ofGetWindowHeight()/2);
		ofPopStyle();
	}

	if(player2->score > player1->score && asteroids.size() == 0) {
		ofPushStyle();
			ofSetColor(255);
			ofDrawBitmapString("Player 2 wins", ofGetWindowWidth()/2,ofGetWindowHeight()/2);
		ofPopStyle();
	}

}

//-------------------------------------------------------------
void ofApp::loadAsteroids()
{
	// Load the file where asteroids definitions are saved
	if(XML.loadFile("asteroids.xml"))
	{
		cout << "Load Asteroids definition xml" << endl;
	}
	else
	{
		cout << "unable to load asteroids.xml check data/ folder" << endl;
	}

	// Check if there is the main xml tag defined or not (as an entry point)
	int areAsteroids = XML.getNumTags("asteroids");
	if(areAsteroids > 0)
	{
		cout << "Found asteroids defined" << endl;
		XML.pushTag("asteroids");

		// Parse all the asteroid definitions
		int numAsteroids = XML.getNumTags("asteroid");
		if(numAsteroids > 0)
		{
			cout << "There are " << numAsteroids << " asteroids defined" << endl;

			for(int i = 0; i < numAsteroids; i++)
			{
				XML.pushTag("asteroid", i);
				vector<ofPoint> tempDefinition;

				int numPoints = XML.getNumTags("point");
				cout << " defined by " << numPoints << " points" << endl;

				// Create a vector for each asteroid definition with the points that define its shape
				for(int i = 0; i < numPoints; i++)
				{
					cout << "  " << XML.getAttribute("point", "x", 0, i) << "x" << XML.getAttribute("point", "y", 0, i) << endl;

					tempDefinition.push_back(ofPoint(XML.getAttribute("point", "x", 0, i), XML.getAttribute("point", "y", 0, i)));
				}

				asteroidsDefinitions.push_back(tempDefinition);
				XML.popTag();
			}
		}
		XML.popTag();
	}
}

//-------------------------------------------------------------
void ofApp::generateAsteroids(int numAsteroids)
{
	float randSize = 0.0f;

	for ( int i = 0; i < numAsteroids; i++ ) 
	{
		// Logic to generate the Asteroids in different sizes
		// these are right now 120, 80, 40 and 20 (minimum)
		int randValue = ofRandom(0, 3);
		if(randValue == 2){
			randSize = 120;
		}
		else 
		{
			if(randValue == 1)
			{
				randSize = 80;
			}
			else 
			{
				randSize = 40;
			}
		}

		int asteroidShape = ofRandom(asteroidsDefinitions.size());

		Asteroid* temp = new Asteroid();
		temp->setup(asteroidsDefinitions.at(asteroidShape), randSize, ofRandom(100, 250), ofRandom(-PI, PI), 
			             ofPoint(ofRandom(0, ofGetWidth()), ofRandom(0, ofGetHeight())), 
						 ofPoint(ofRandom(-1, 1), ofRandom(-1, 1)).normalize());
		asteroids.push_back(temp);
	}
}



//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y )
{

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{ 

}

void ofApp::spaceShipFiresEvent(ofSpaceShipFireEventArgs &e)
{
	Bullet* bullet = new Bullet();
	//ofPoint spaceShipPosition, ofPoint spaceShipDirection, float size, float speed, float lifeTime);
	ofPoint position = player1->getPosition();
	position -= player1->getDirection()*11;
	bullet->setup(position, player1->getDirection(), 2, 300, 100,"player1");
	bullets.push_back(bullet);
}

void ofApp::spaceShipFiresEvent2(ofSpaceShipFireEventArgs &e)
{
	Bullet* bullet = new Bullet();
	//ofPoint spaceShipPosition, ofPoint spaceShipDirection, float size, float speed, float lifeTime);
	ofPoint position = player2->getPosition();
	position -= player2->getDirection()*11;
	bullet->setup(position, player2->getDirection(), 2, 300, 100,"player2");
	bullets.push_back(bullet);
}

void ofApp::addAsteroid(vector<Asteroid*> &vec, vector<Asteroid*>::iterator &it, Asteroid* asteroid)
{
    const size_t diff = it-vec.begin();
    vec.push_back(asteroid);
    it = vec.begin()+diff;
}

void ofApp::eraseBullet(vector<Bullet*> &vec, vector<Bullet*>::iterator &it)
{
	vector<Bullet*>::iterator new_it = it++;
    vec.erase(it); 
    it = new_it;
}