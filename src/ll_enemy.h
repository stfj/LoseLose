/*
 *  ll_enemy.h
 *  Lose/Lose
 *
 *  Created by Zach Gage on 9/13/09.
 *  Copyright 2009 stfj. All rights reserved.
 *
 */

#ifndef LL_ENEMY
#define LL_ENEMY

#include "ofMain.h"

#define MOVE_STRAIGHT 1
#define MOVE_SIN 2
#define MOVE_SEEK 3

class ll_enemy 
{
	public:
	
	ll_enemy(string _path, float speedPlus);
	~ll_enemy();
	
	void draw();
	void update(int playerX);
	
	void generatePose();
	
	bool dead;
	ofPoint pos;
	float xSpeed;
	int xSpeedMult;
	int ySpeed;
	bool rush;
	
	int ySize;
	int xSize;
	
	float sinX;
	
	int r;
	int g;
	int b;
	
	int movementType;
	
	int blockSize;
	
	string path;
	string name;
	string ext;
	
	float * sequence;
	
	vector<ofPoint> blocks;
	vector<ofPoint> blocks2;
	
	int pose;
	int poseCounter;
	int hp;
};

#endif
