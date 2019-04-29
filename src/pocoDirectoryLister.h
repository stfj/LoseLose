#ifndef _OFX_POCO_DIRECTORY_LISTER
#define _OFX_POCO_DIRECTORY_LISTER

#include "ofMain.h"

//#include "ofAddons.h"

#include "Poco/DirectoryIterator.h"
#include "Poco/File.h"
#include "Poco/Path.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"

#include <vector>

using Poco::DirectoryIterator; 
using Poco::File; 
using Poco::Path; 
using Poco::DateTimeFormatter; 
using Poco::DateTimeFormat; 

class ofxPocoDirectoryLister
{	
	public:
	ofxPocoDirectoryLister(); //constructor
	~ofxPocoDirectoryLister(); //destructor
	
	void setExtensionToLookFor(string extension)
	{
		extensionToLookFor = extension;
	}
	string getExtensionToLookFor()
	{
		return extensionToLookFor;
	}
	void setPath(Path _p)
	{
		currentPath = _p;
	}
	Path getCurrentPath()
	{
		return currentPath;
	}
	long getNumberOfFiles()
	{
		refreshFiles();
		return numFiles;
		//return currentFiles.size();
	}
	
	void listFiles(); //prints out all the files in the current path, only to be called after refreshing
	
	
	Path getPath(long l);
private:
	void refreshFiles(); //refreshes all the files in the current path
	Path			currentPath;			//the current path of the directory lister
	std::vector <Path>	currentFiles; //a vector containing all the files in our current path
	string extensionToLookFor;		//a string representing the extension we are looking for
	int numFiles;
};

#endif
	
