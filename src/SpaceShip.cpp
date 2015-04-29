#include "SpaceShip.h"

SpaceShip::SpaceShip()
{
	position.x = 0;
	position.y = 0;
	player1 = player2 = false;
	direction = ofPoint(0, 0);

	rotation = 0;
	score = 0;

	ofEvent<ofSpaceShipFireEventArgs> spaceShipFires = ofEvent<ofSpaceShipFireEventArgs>();
}

SpaceShip::~SpaceShip()
{
	
}


bool SpaceShip::setup()
{
	// A maximum later size for the SpaceShip
	size = 30;

	// Initialize it in the middle of the screen
	position.x = ofGetWidth()/2 - size / 2;
	position.y = ofGetHeight()/2 - size / 2;
		
	rotation = PI/2;
	direction.x = cos(rotation);
	direction.y = sin(rotation);

	speed = 0;
	
	turnLeft = turnRight = thrust = backThrust = isFiring = false;

	bulletCount = 0;

	timeIsDestroyed = -1.0;

	ofRegisterKeyEvents(this);

	return true;
}


void SpaceShip::update(float elapsedTime)
{
	if(turnLeft)
	{
		addRotation(-0.090);

	}

	if(turnRight)
	{
		addRotation(0.090);
	}

	if(thrust){
		addThrust(5);
	} 
	else 
	{
		addThrust(-2);
	}

	if(backThrust)
	{
		addThrust(-1.5);
	}
	
	if(thrust || turnLeft || turnRight)
	{
		direction.x = cos(rotation);
		direction.y = sin(rotation);
	}

	if(isFiring)
	{
		if(bulletCount < 1)
		{
			ofSpaceShipFireEventArgs e = {position, ofPoint(cos(rotation), sin(rotation))};
			ofNotifyEvent(spaceShipFires, e, this);
			bulletCount++;
		}
	}
	
	if(timeIsDestroyed >= 0)
	{
		if(timeIsDestroyed >= 1)
		{
			resetSpaceShip();
		}
		else
		{
			timeIsDestroyed += 0.05;
		}
		
	}
	else
	{
		position += -(direction) * speed * elapsedTime;
	}

	
	marginsWrap();
}

void SpaceShip::draw(bool debug)
{
	ofPushStyle();
	ofPushMatrix();
		ofSetColor(255);
		ofTranslate(position.x,position.y,0);
		ofRotate((rotation - PI/2) / PI * 180, 0,0,1);

		if(debug)
		{
			ofPushStyle();
				ofNoFill();
				ofCircle(0, 0,size);
			ofPopStyle();
		}

		ofRotate(-90);
		ofTranslate(-size/2, -size/2);

		if(timeIsDestroyed < 0)
		{
			ofLine(0,0,size,size/2);
			ofLine(size,size/2,0,size);
			ofLine(size*0.1,size*0.1,size*0.1,size-size*0.1);
		}
		else
		{
			ofLine(ofPoint(0, 0), ofPoint(size*timeIsDestroyed, 0));
			ofLine(ofPoint(0, 0), ofPoint(size*timeIsDestroyed, size*timeIsDestroyed));

			ofLine(ofPoint(0, 0), ofPoint(-size*timeIsDestroyed, 0));
			ofLine(ofPoint(0, 0), ofPoint(0, -size*timeIsDestroyed));

			ofLine(ofPoint(0, 0), ofPoint(0, size*timeIsDestroyed));
			ofLine(ofPoint(0, 0), ofPoint(-size*timeIsDestroyed, size*timeIsDestroyed));

			ofLine(ofPoint(0, 0), ofPoint(size*timeIsDestroyed, -size*timeIsDestroyed));
			ofLine(ofPoint(0, 0), ofPoint(-size*timeIsDestroyed, -size*timeIsDestroyed));
		}
		if (thrust) 
		{
			ofLine(size * 0.1, size * 0.1 + 2, size*0.1 - size*0.2, (size - size*0.1) / 2);
			ofLine(size * 0.1 - size*0.2, (size - size*0.1) / 2, size*0.1, size - size*0.1 - 2);
		}
	ofPopMatrix();
	ofPopStyle();
}

void SpaceShip::resetSpaceShip()
{
	position.x = ofGetWidth()/2 - size / 2;
	position.y = ofGetHeight()/2 - size / 2;

	rotation = PI/2;
	speed = 0;
	timeIsDestroyed = -1;

	turnLeft = turnRight = thrust = backThrust = isFiring = false;
}

void SpaceShip::addThrust(float thrust)
{
	speed += thrust;

	if(speed > 400)
	{
		speed = 400;
	}
	else 
	{
		if(speed < 0.0 && !backThrust)
		{
			speed = 0.0;
		}
	}
	
	if(backThrust) 
	{
		speed += thrust;

		if(speed < 0.0)
		{
			speed = 0.0;
		}
	}
}

void SpaceShip::keyPressed(ofKeyEventArgs & args)
{
	if(player1){
		switch(args.key){

				case OF_KEY_UP:
					thrust = true;
					break;
				case OF_KEY_LEFT:
					turnLeft = true;
					break;
				case OF_KEY_RIGHT:
					turnRight = true;
					break;
				case OF_KEY_DOWN:
					backThrust = true;
					break;
				case ' ':
					isFiring = true;
					break;
			}
	}

	if(player2){
		switch(args.key){
			case 'w':
				thrust = true;
				break;
			case 'a':
				turnLeft = true;
				break;
			case 'd':
				turnRight = true;
				break;
			case 's':
				backThrust = true;
				break;
			case 'g':
				isFiring = true;
				break;
		}
	}
}

void SpaceShip::keyReleased(ofKeyEventArgs & args)
{

	if(player1){
		switch(args.key){

		
				case OF_KEY_UP:
					thrust = false;
					break;
				case OF_KEY_LEFT:
					turnLeft = false;
					break;
				case OF_KEY_RIGHT:
					turnRight = false;
					break;
				case OF_KEY_DOWN:
					backThrust = false;
					break;
				case ' ':
					isFiring = false;
					bulletCount = 0;
					break;
		}
	}

	if(player2){	

		switch(args.key){
			case 'w':
				thrust = false;
				break;
			case 'a':
				turnLeft = false;
				break;
			case 'd':
				turnRight = false;
				break;
			case 's':
				backThrust = false;
				break;
			case 'g':
				isFiring = false;
				bulletCount = 0;
				break;
		}
	}
}

void SpaceShip::setIsBeingDestroyed()
{
	timeIsDestroyed = 0.0;
}

float SpaceShip::getTimeIsDestroyed()
{
	return timeIsDestroyed;
}