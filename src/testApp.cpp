#include "testApp.h"
#include "zg_of_util.h"

testApp::~testApp()
{
	if(score>0)
	{
		string submitHighScores = "HIGHSCORELINK?action=submit&admin_user=USER&admin_pass=PASSWORD&name="+playerName+"&score="+ofToString(score)+"&access_code=CODE";
		
		if(LIVE)
		{
			HTTPStreamFactory::registerFactory();
			
			//specify out url and open stream
			URI uri(submitHighScores);      
			std::auto_ptr<std::istream> pStr(URIStreamOpener::defaultOpener().open(uri));
			
			string str;       
			StreamCopier::copyToString(*pStr.get(), str);
		
		
			cout<<str<<endl;
		}
	}
	if(gameLost)
		deleteSelf();
}
//--------------------------------------------------------------
void testApp::setup()
{
	ofBackground(0,0,0);
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
		
	LIVE = false;
	gameWon = false;
	gameLost = false;
	
	DIR = new ofxDirList();
	
	homeDirectory = getHomeDir();
	
	playerName = homeDirectory.substr(homeDirectory.rfind("/")+1,  homeDirectory.size()-homeDirectory.rfind("/")-1);
		
	cout<<"Home Directory: "<<homeDirectory<<endl;
	cout<<"Player Name: "<<playerName<<endl;
	
	//test code -----------------------------------
	//homeDirectory+="/test/test2";
	
	score = 0;
	
	player.set(ofGetWidth()/2,ofGetHeight()-64);
	
	mvUp = false;
	mvLf = false;
	mvRt = false;
	mvDn = false;
	shoot = false;
	
	playerSpeed = 5;
	bulletSpeed = 16;
	bulletLag = 6;
	bulletTimer=0;
	
	enemyLag=30;
	enemyTimer=0;
	
	playerLaser.loadSound("laser3.wav");
	playerLaser.setMultiPlay(true);
	playerLaser.setVolume(0.1);
	
	explosion.loadSound("explosion.wav");
	explosion.setMultiPlay(true);
	explosion.setVolume(0.3);
	
	hurt.loadSound("hurt.wav");
	hurt.setMultiPlay(true);
	hurt.setVolume(0.1);
	
	music.loadSound("m3d049_bopSrc_03_hwyChipmusik_by_xik_vbr.mp3");
	music.setLoop(true);
	music.setVolume(0.3);
	
	ship[0].loadImage("ship1.png");
	ship[1].loadImage("ship2.png");
	ship[2].loadImage("ship3.png");
	
	stars[0].loadImage("stars1.png");
	stars[1].loadImage("stars2.png");
	
	starPos[0] = starPos[1]= 0;
	
	speedPlus=0;
	
	font = new ofTrueTypeFont();
	font->loadFont("victor-pixel.ttf", 16);
	
	smallFont = new ofTrueTypeFont();
	smallFont->loadFont("PixelSplitter-Bold.ttf", 8);
	
	if(LIVE)
	{
		gamestate=GAME_STATE_WARNING_MSG;
		startTimer=200;
	}
	else {
		gamestate=GAME_STATE_WARNING_MSG;
		startTimer=200;
	}

}

//--------------------------------------------------------------
void testApp::update()
{
	if(startTimer>0)
	{
		startTimer--;
	}
	if(gamestate==GAME_STATE_PLAYING)
	{
		
		if(!gameWon && !gameLost)
		{
			updatePlayer();
		}
		
		updateBullets();
		updateEnemies();
		
		for(int i=enemyParts.size()-1;i>=0;i--)
		{
			enemyParts[i].update();
			if(enemyParts[i].pos.y>ofGetHeight())
				enemyParts.erase(enemyParts.begin()+i);
		}
	}
}

//--------------------------------------------------------------
void testApp::draw()
{
	ofEnableAlphaBlending();
	
	
	stars[0].draw(0,starPos[0]);
	stars[0].draw(0,starPos[0]-stars[0].height);
	starPos[0]+=0.5;
	if(starPos[0]==stars[0].height)
		starPos[0]=0;
	
	stars[1].draw(0,starPos[1]);
	stars[1].draw(0,starPos[1]-stars[1].height);
	starPos[1]++;
	if(starPos[1]==stars[1].height)
		starPos[1]=0;
	
	
	if(gamestate==GAME_STATE_WARNING_MSG)
	{
		ofSetColor(255,0,0);
		
		if(startTimer==0)
		{
			font->drawString("!! Warning !! \n\nplaying lose/lose will likely\n\n\n\n\n\nI take no responsibility for loss of\nany data//\n\nyou have been warned//\n\n\n\npress any key to continue//", 16, 32);
		}
		else {
			font->drawString("!! Warning !! \n\nplaying lose/lose will likely\n\n\n\n\n\nI take no responsibility for loss of\nany data//\n\nyou have been warned//\n\n\n\n", 16, 32);
		}

		
		ofSetColor(255,255,255);
		font->drawString("\n\n\nresult in files on your hard drive\nbeing deleted//\n\nkilling in lose/lose deletes your files", 16, 32);
		
		ofSetColor(255,0,0);
	}
	
	if(gamestate==GAME_STATE_TITLE)
	{
		ofSetColor(255,255,255);
		font->drawString("lose/lose",16,32);
		ofSetColor(100,100,100);
		font->drawString("control with arrow keys",ofGetWidth()/2-font->stringWidth("control with arrow keys")/2,ofGetHeight()/2-8-96);
		font->drawString("shoot with space",ofGetWidth()/2-font->stringWidth("shoot with space")/2,ofGetHeight()/2-8-64);
		ofSetColor(255,255,255);
		font->drawString("press any key to begin",ofGetWidth()/2-font->stringWidth("press any key to begin")/2,ofGetHeight()/2-8);
		smallFont->drawString("\n\na game of consequences//", 16, 24);
		ofSetColor(150,150,200);// + 6 \n's for 800 height
		smallFont->drawString("\n\n\n\nby zach gage // stfj.net // 2009\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nmade with openFrameworks//\naudio from gratisvibes.com//\nEnemy appearance code made with david wicks in 2007//", 16, 24);
		ofSetColor(255,255,255);
	}
	
	if(gamestate==GAME_STATE_PLAYING)
	{
		for(int i=deadTypes.size()-1;i>=0;i--)
		{
			if(deadTypes[i].time<30)
			{
				smallFont->drawString(deadTypes[i].text, deadTypes[i].point.x-smallFont->stringWidth(deadTypes[i].text)/2, deadTypes[i].point.y);
				deadTypes[i].time++;
			}
			else
			{
				deadTypes.erase(deadTypes.begin()+i);
			}
		}
		
		
		
		
		for(int i=0;i<enemyParts.size();i++)
		{
			enemyParts[i].draw();
		}
		
		
		ofSetColor(255,255,255);
		
		if(!gameLost)
		{
			if(mvLf && !mvRt)
				ship[1].draw(player.x,player.y);
			else if(mvRt && !mvLf)
				ship[2].draw(player.x,player.y);
			else {
				ship[0].draw(player.x,player.y);
			}
		}

		ofSetColor(255,255,0);
		
		for(int i=0;i<playerBullets.size();i++)
		{
			ofRect(playerBullets[i].x,playerBullets[i].y,1,4);
		}
		
		for(int i=0;i<enemies.size();i++)
		{
			enemies[i]->draw();
		}
		
		ofSetColor(255,255,255);
		font->drawString(" score / "+ofToString(score), 0, 16);
		
		if(gameLost)
			font->drawString("you lost", ofGetWidth()/2 - font->stringWidth("you lost")/2, ofGetHeight()/2);
		
		if(gameWon)
			font->drawString("you won!", ofGetWidth()/2 - font->stringWidth("you won!")/2, ofGetHeight()/2);
	}
}
//--------------------------------------------------------------
void testApp::updateBullets()
{
	for(int i=playerBullets.size()-1;i>=0;i--)
	{
		playerBullets[i].y-=bulletSpeed;
		
		if(playerBullets[i].y<0)
			playerBullets.erase(playerBullets.begin()+i);
	}
}
//--------------------------------------------------------------
void testApp::updatePlayer()
{
	if(mvUp)
		player.y = MAX(player.y-playerSpeed,0);
	if(mvDn)
		player.y = MIN(player.y+playerSpeed,ofGetHeight()-32);
	if(mvLf)
		player.x = MAX(player.x-playerSpeed,0);
	if(mvRt)
		player.x = MIN(player.x+playerSpeed,ofGetWidth()-32);
	
	if(shoot)
		addPlayerBullet();
	
	for(int e=0; e<enemies.size();e++)
	{
		if(enemies[e]->pos.x+enemies[e]->xSize*enemies[e]->blockSize/2 > player.x)
			if(enemies[e]->pos.x-enemies[e]->xSize*enemies[e]->blockSize/2 < player.x+32)
				if(enemies[e]->pos.y+enemies[e]->xSize*enemies[e]->blockSize/2 >player.y)
					if(enemies[e]->pos.y-enemies[e]->xSize*enemies[e]->blockSize/2<player.y+32)
					{
						gameLost=true;
						explosion.play();
						for(int x=0;x<16;x++)
						{
							for(int y=0;y<16;y++)
							{
								enemyParts.push_back(particle());
								enemyParts[enemyParts.size()-1].setInitialCondition(player.x+x*2,player.y+y*2,8-x,8-y);
								enemyParts[enemyParts.size()-1].r = 255;
								enemyParts[enemyParts.size()-1].g = 255;
								enemyParts[enemyParts.size()-1].b = 255;
								enemyParts[enemyParts.size()-1].size= 2;
								enemyParts[enemyParts.size()-1].addForce(0,ofRandomf()+0.5);
							}
						}
					}
	}
}
//--------------------------------------------------------------
void testApp::updateEnemies()
{
	if(enemyTimer<=0 && !gameLost && !gameWon)
	{
		newEnemy();
		enemyTimer=enemyLag;
	}
	else {
		enemyTimer--;
	}

	
	for(int i=enemies.size()-1;i>=0;i--)
	{
		enemies[i]->update(player.x);
		
		//collision check
		for(int b=0;b<playerBullets.size();b++)
		{
			//collisionCheck
			if(playerBullets[b].y>enemies[i]->pos.y)
				if(playerBullets[b].y<enemies[i]->pos.y+enemies[i]->ySize*enemies[i]->blockSize)
					if(playerBullets[b].x>enemies[i]->pos.x-enemies[i]->xSize*enemies[i]->blockSize/2)
						if(playerBullets[b].x<enemies[i]->pos.x+enemies[i]->xSize*enemies[i]->blockSize/2)
						{
							enemies[i]->hp--;
							playerBullets[b].y=-64;
							if(enemies[i]->hp<=0)
							{
								enemies[i]->dead=true;
							}
							else
							{
								hurt.play();
							}
						}
		}
		
		if(enemies[i]->dead)
		{
			explosion.play();
			score++;
			
			deadTypes.push_back(fadePoint());
			deadTypes[deadTypes.size()-1].text=enemies[i]->ext;
			deadTypes[deadTypes.size()-1].time=0;
			deadTypes[deadTypes.size()-1].point=ofPoint(enemies[i]->pos.x,enemies[i]->pos.y);
			
			for(int b=0;b<enemies[i]->blocks.size();b++)
			{
				enemyParts.push_back(particle());
				enemyParts[enemyParts.size()-1].setInitialCondition(enemies[i]->pos.x+enemies[i]->blocks[b].x,enemies[i]->pos.y+enemies[i]->blocks[b].y,ofRandom(-1,3),-enemies[i]->blocks[b].y/2);
				enemyParts[enemyParts.size()-1].r = enemies[i]->r;
				enemyParts[enemyParts.size()-1].g = enemies[i]->g;
				enemyParts[enemyParts.size()-1].b = enemies[i]->b;
				enemyParts[enemyParts.size()-1].size= enemies[i]->blockSize;
				enemyParts[enemyParts.size()-1].addForce(0,ofRandomf()+0.5);
				
				enemyParts.push_back(particle());
				enemyParts[enemyParts.size()-1].setInitialCondition(enemies[i]->pos.x-enemies[i]->blocks[b].x-enemies[i]->blockSize,enemies[i]->pos.y+enemies[i]->blocks[b].y,ofRandom(1,-3),-enemies[i]->blocks[b].y/2);
				enemyParts[enemyParts.size()-1].r = enemies[i]->r;
				enemyParts[enemyParts.size()-1].g = enemies[i]->g;
				enemyParts[enemyParts.size()-1].b = enemies[i]->b;
				enemyParts[enemyParts.size()-1].size= enemies[i]->blockSize;
				enemyParts[enemyParts.size()-1].addForce(0,ofRandomf()+0.5);
			}
			
			
			deleteFile(enemies[i]->path);
			enemies.erase(enemies.begin()+i);
		}
		else if(enemies[i]->pos.y>ofGetHeight())
		{
			enemies.erase(enemies.begin()+i);
		}

	}
}

//--------------------------------------------------------------
void testApp::addPlayerBullet()
{
	if(bulletTimer<=0)
	{
		playerBullets.push_back(ofPoint(player.x+15,player.y));
		bulletTimer=bulletLag;
		playerLaser.play();
	}
	else
	{
		bulletTimer--;
	}
}

//--------------------------------------------------------------
void testApp::newEnemy()
{
	ll_enemy * newEnemy;
	newEnemy = new ll_enemy(getNextFile(),speedPlus);
	enemies.push_back(newEnemy);
	speedPlus+=0.01;
}

//--------------------------------------------------------------
string testApp::getNextFile()
{
	int curIndex = dirIndex[dirIndex.size()-1];
	if(! gameWon)
	{
		if( curIndex < dirSize ) // if we are still within the directory
		{
			if( isDirectory(DIR->getPath(curIndex)) ) // if the file we are looking at is a directory
			{
				dirIndex[dirIndex.size()-1]++; // move the iterator to the next file
				nextDirectory(DIR->getPath(curIndex)); // enter the directory
				return getNextFile(); // recurse through the directory
			}
			else // great! we are looking at a file
			{
				dirIndex[dirIndex.size()-1]++; // move the iterator to the next file
				
				return DIR->getPath(curIndex); // return the name of the previous file
			}

		}
		else // we have gone out of bounds; 
		{
			prevDirectory(); // go back a directory
			return getNextFile(); //recurse through the old directory
		}

	}
	else 
	{
		return "";
	}
	
}

//--------------------------------------------------------------
string testApp::getHomeDir()
{
	string dir;
	
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	CFURLRef resourcesURL = CFBundleCopyBundleURL(mainBundle);
	CFStringRef str = CFURLCopyFileSystemPath( resourcesURL, kCFURLPOSIXPathStyle );
	CFRelease(resourcesURL);
	char path[PATH_MAX];
	
	CFStringGetCString( str, path, FILENAME_MAX, kCFStringEncodingASCII );
	CFRelease(str);
	
	dir = path;
	
	int homeDirPos = getNumberedPositionOfChar("/", 3, &dir);
	dir = dir.substr(0,homeDirPos);
	
	dirIndex.push_back(0);
	
	dirSize = DIR->listDir(dir);
	
	
	return dir;
}
//--------------------------------------------------------------
bool testApp::isDirectory(string path)
{
	bool isADirectory = false;
	
	int rFindDot = path.rfind(".");
	
	if(rFindDot == -1) // cannot find a .  may be a directory
	{
		dirList.setPath(path); //check better to see if its a directory
		int insideFiles = dirList.getNumberOfFiles();
		
		if(insideFiles != 0)
			isADirectory = true;
		
		if(insideFiles > 500)
			isADirectory = false;
	}
	
	if(path.find(".app")!=-1) //apps should be deleted as a whole
		isADirectory=false;
	if(path.find(".App")!=-1) //apps should be deleted as a whole
		isADirectory=false;
	if(path.find(".APP")!=-1) //apps should be deleted as a whole
		isADirectory=false;
	
	if(path.find("font")!=-1) //fonts give issues. delete all at once
		isADirectory=false;
	if(path.find("Font")!=-1) //fonts give issues. delete all at once
		isADirectory=false;
	if(path.find("FONT")!=-1) //fonts give issues. delete all at once
		isADirectory=false;
	
	
	return isADirectory;
}

//--------------------------------------------------------------
void testApp::prevDirectory()
{
	delete DIR;
	DIR = new ofxDirList();
	
	//cout<<homeDirectory<<endl;
	
	homeDirectory = homeDirectory.substr(0, homeDirectory.rfind("/"));
	dirSize = DIR->listDir(homeDirectory);
	
	cout<<"prevDir: "<<homeDirectory<<endl;
	if(dirIndex.size()>0)
	{
		dirIndex.erase(dirIndex.end()-1);
		
		if(dirIndex.size()==0)
		{
			gameWon = true;
			cout<<"you win!"<<endl;
		}
	}
}

//--------------------------------------------------------------
void testApp::nextDirectory(string path)
{
	delete DIR;
	DIR = new ofxDirList();
	dirSize = DIR->listDir(path);
	
	if(dirSize != 0)
	{
		//cout<<"nextDir: "<<path<<endl;
		homeDirectory = path;
		dirIndex.push_back(0);
	}
	else {
		dirSize = DIR->listDir(homeDirectory);		
	}

}

//--------------------------------------------------------------
void testApp::deleteFile(string path)
{
	int loadCmd;

	string dangerous;
	
	dangerous = "rm -r \"";
	dangerous += path;
	dangerous += "\"";	
	
	if(LIVE)
		loadCmd = system(dangerous.c_str()); //uncomment to make live
	
	cout<<dangerous<<endl;
	
	
	
	//check to remove directory
	
	int numberOfSubFolders = getNumOccurance("/", path);
	string subDirectory = path.substr(0, getNumberedPositionOfChar("/", numberOfSubFolders, &path));
	
	if(delDir.listDir(subDirectory)==0)
	{
		dangerous = "rm -r \"";
		dangerous+=subDirectory;
		dangerous += "\"";
		
		if(LIVE)
			loadCmd = system(dangerous.c_str()); //uncomment to make live
		
		cout<<"deleted sub directory"<<endl;
		cout<<dangerous<<endl;
	}
}
//--------------------------------------------------------------
void testApp::deleteSelf()
{
	string dir;
	
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	CFURLRef resourcesURL = CFBundleCopyBundleURL(mainBundle);
	CFStringRef str = CFURLCopyFileSystemPath( resourcesURL, kCFURLPOSIXPathStyle );
	CFRelease(resourcesURL);
	char path[PATH_MAX];
	
	CFStringGetCString( str, path, FILENAME_MAX, kCFStringEncodingASCII );
	CFRelease(str);
	
	dir = path;
	
	string dangerous;
	
	dangerous = "rm -r \"";
	dangerous+=path;
	dangerous += "\"";
	
	int loadCmd;
	
	if(LIVE)
		loadCmd = system(dangerous.c_str());
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	if(gamestate==GAME_STATE_PLAYING)
	{
		if(key==357)
			mvUp = true;
		else if(key==358)
			mvRt = true;
		else if(key==359)
			mvDn = true;
		else if(key==356)
			mvLf = true;
		else if(key==32)
		{
			bulletTimer=0;
			shoot = true;
		}
	}
	else if(startTimer==0)
	{
		gamestate++;
		if(gamestate==GAME_STATE_PLAYING)
			music.play();
	}

}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{
	if(gamestate==GAME_STATE_PLAYING)
	{
		if(key==357)
			mvUp = false;
		else if(key==358)
			mvRt = false;
		else if(key==359)
			mvDn = false;
		else if(key==356)
			mvLf = false;
		else if(key==32)
			shoot = false;
	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

