#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxDirList.h"
#include "pocoDirectoryLister.h"
#include "particle.h"
#include "ll_enemy.h"
#include "ofxHttpUtils.h"

#include "Poco/URIStreamOpener.h"
#include "Poco/StreamCopier.h"
#include "Poco/Path.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include "Poco/Net/HTTPStreamFactory.h"

using Poco::URIStreamOpener;
using Poco::StreamCopier;
using Poco::Path;
using Poco::URI;
using Poco::Exception;
using Poco::Net::HTTPStreamFactory;

#define GAME_STATE_WARNING_MSG 1
#define GAME_STATE_TITLE 2
#define GAME_STATE_PLAYING 3

struct fadePoint {
	ofPoint point;
	int time;
	string text;
};

class testApp : public ofBaseApp{

	public:
	
		~testApp();
	
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
	
		void updatePlayer();
		void updateBullets();
		void updateEnemies();
	
		void addPlayerBullet();
	
	
		void deleteFile(string path);
		void deleteSelf();
	
		string getHomeDir();
		string getNextFile();
		
		bool isDirectory(string path);
	
		void prevDirectory();
		void nextDirectory(string path);
		void newEnemy();
	
		ofxDirList * DIR;
	
		ofxDirList delDir;
		int dirSize;
	
		ofxPocoDirectoryLister dirList;
	
		int gamestate;
	
		vector<int> dirIndex;
	
		vector<ll_enemy *> enemies;
	
		vector<ofPoint> playerBullets;
	
		vector<particle> enemyParts;
	
		vector<fadePoint> deadTypes;
	
		ofPoint player;
	
		string homeDirectory;
		string playerName;
	
		ofxHttpUtils web;
	
		bool LIVE;
		bool gameWon;
		bool gameLost;
	
		bool mvUp;
		bool mvLf;
		bool mvRt;
		bool mvDn;
		bool shoot;
		
		int score;
		int bulletTimer;
		int enemyTimer;
	
		ofTrueTypeFont * font;
		ofTrueTypeFont * smallFont;
	
		float speedPlus;
	
		ofSoundPlayer playerLaser;
		ofSoundPlayer explosion;
		ofSoundPlayer hurt;
		ofSoundPlayer music;
		ofImage ship[3];
	
		ofImage stars[2];
		float starPos[2];
	
		//---settings---
		int playerSpeed;
		int bulletSpeed;
		int bulletLag;
		int enemyLag;
	
		int startTimer;
};

#endif
