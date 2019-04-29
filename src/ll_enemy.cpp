/*
 *  ll_enemy.cpp
 *  Lose/Lose
 *
 *  Created by Zach Gage on 9/13/09.
 *  Copyright 2009 stfj. All rights reserved.
 *
 */

#include "ll_enemy.h"

ll_enemy::ll_enemy(string _path, float speedPlus)
{
	path = _path;
	dead = false;
	rush = false;
	
	name = path.substr(path.rfind("/")+1,  path.size()-path.rfind("/")-1);
	
	
	
	//color, size, and speed values based on file type
	if(name.size()>3)
	{
		ofSeedRandom(name[name.size()-1]*name[name.size()-2]*name[name.size()-3]);
		ext = path.substr(path.rfind(".")+1, path.size()-path.rfind(".")-1);
	}
	else {
		ext = "nan";
	}

		
	r = ofRandom(50,255);
	g = ofRandom(50,255);
	b = ofRandom(50,255);
	xSize = ofRandom(10,32);
	ySize = ofRandom(10,32);

	xSpeed=ofRandom(0,0.05);
	ySpeed=ofRandom(1,3)+speedPlus;
	
	xSpeedMult = ofRandom(2,4);
	
	hp = ofRandom(1,8);
	
	blockSize=2;
	
	movementType = round(ofRandom(0,3));
	
	//random poses per name before file type
	int r=1;
	for(int i=0;i<name.size()-4;i++)
	{
		r*=name[i];
	}
	
	ofSeedRandom(r);
	
	generatePose();
	
	//totally random position-------
	ofSeedRandom();
	pos.set(ofRandom(0,ofGetWidth()-32),-ySize);
	
	sinX=ofRandomf();
	//xSize=32;
	//ySize=32;
	pose=0;
	poseCounter=30;
}

void ll_enemy::generatePose()
{
	int numPieces = 0;
	int maxPieces = 90;
	
	int seqSize = (xSize*ySize)/2;
	sequence = new float[seqSize];
	
	for(int i=0;i<seqSize;i++)
	{
		sequence[i] = ofRandom(0,2);
	}
	
	int xPos = 0;
	int yPos = 0;
	
	for(int i=0; i<seqSize; i++)
	{
		float sequenceValue = sequence[i];
		
		if(xPos <= xSize/4)
			sequenceValue += 0.3;
		
		if(yPos <= ySize/4 ){
			//weight near the top
			sequenceValue += 0.3;
		}

		if(yPos >= ySize - ySize/4){
			sequenceValue -= 0.05;
		}
		//if the value is high enough, add a block to our alien
		if(sequenceValue >= 1)
		{
			//place a block
			if(numPieces < maxPieces){
				blocks.push_back(ofPoint(xPos*blockSize,yPos*blockSize));
				numPieces++;
			}
		}
		
		xPos++;
		if(xPos >= xSize/2){
			xPos = 0;
			yPos++;
		}
	}
	
	for( int j = blocks.size() - 1 ; j >= 0 ; j-- )
	{
		bool metNeighbors = false;
		//check all other blocks for proximity
		for( int i = 0; i < blocks.size(); i++ )
		{
			if( j != i ){
				float distance = sqrt(( blocks[j].x - blocks[i].x ) * ( blocks[j].x - blocks[i].x ) + ( blocks[j].y - blocks[i].y ) * ( blocks[j].y - blocks[i].y ));
				if( distance < 1.45 * blockSize){
					//multiply by close to sqrt(2)
					metNeighbors = true;
				}
			}
		}
		if(!metNeighbors){
			blocks.erase(blocks.begin()+j);
		}
    }
	
	//second pose::
	
	for(int i=0;i<blocks.size();i++)
	{
		blocks2.push_back(blocks[i]);
	}
	
	for( int i = 0; i < blocks2.size(); i++ ){
		if(i>blocks2.size()/2 && (i+1)%(xSize/2)>0) //this is where i restricted it to the bottom half and not the inside 2 pixels
		{
			float leftSpace = blocks2[i].x - blockSize;
			bool isOccupied = false;
			if(blocks2[i].x > 0){
				//provided we aren't the leftmost pixel,
				//check for open space to the left
				for( int j = 0; j < blocks2.size(); j++ ){
					if( j != i ){
						//
						if(blocks2[j].y == blocks2[i].y){
							if(blocks2[j].x == leftSpace){
								isOccupied = true;
							}
						}
					}
				}
				if(!isOccupied){
					blocks2[i].x -= blockSize;
				}
			}
		}
    }
}

ll_enemy::~ll_enemy(){
	delete[] sequence;
}


void ll_enemy::update(int playerX)
{
	sinX+=xSpeed;
	switch (movementType) {
		case MOVE_STRAIGHT:
			break;
		case MOVE_SIN:
			pos.x=CLAMP(pos.x+sin(sinX)*xSpeedMult,xSize*blockSize/2,ofGetWidth()-xSize*blockSize/2);
			break;
		case MOVE_SEEK:
			pos.x+= float(playerX-pos.x)/40;
			if(abs((int(pos.x-playerX)))<16)
				rush=true;
			break;
		default:
			break;
	}
	
	if(rush)
		pos.y+=ySpeed*5;
	else
		pos.y+=ySpeed;
	
	if(poseCounter<0)
	{
		poseCounter=30;
		if(pose<=0)
			pose=1;
		else {
			pose=0;
		}

	}
	else
	{
		poseCounter--;
	}
}

void ll_enemy::draw()
{
	ofSetColor(r,g,b);
	if(pose==0)
	{
		for(int i=0;i<blocks.size();i++)
		{
			ofRect(pos.x+blocks[i].x,pos.y+blocks[i].y,blockSize,blockSize);
			ofRect(pos.x-blocks[i].x-blockSize,pos.y+blocks[i].y,blockSize,blockSize);
		}
	}
	else {
		for(int i=0;i<blocks2.size();i++)
		{
			ofRect(pos.x+blocks2[i].x,pos.y+blocks2[i].y,blockSize,blockSize);
			ofRect(pos.x-blocks2[i].x-blockSize,pos.y+blocks2[i].y,blockSize,blockSize);
		}
	}
	
	ofSetColor(255,0,0);
	ofRect(pos.x-xSize*blockSize/2, pos.y-5, hp*4, 2);

//	ofRect(pos.x,pos.y,xSize,ySize);
}
