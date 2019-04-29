
#ifndef _ZG_OF_UTIL
#define _ZG_OF_UTIL

#include <fstream>
#include "ofMain.h"

/*
 *  zg_of_util.h
 *  openFrameworks
 *
 *  Created by Zach Gage on 12/8/08.
 *  Copyright 2008 stfj. All rights reserved.
 *
 */

//----------------------------

void replaceMultiBetween(string startChar, string endChar, string replacement, string * data)
{
	//erase commas-------
	
	int startCharIndex=data->find(startChar);
	
	
	while(startCharIndex!=-1)
	{
		string temp = data->substr(startCharIndex,data->size()-startCharIndex);
		int endCharIndex = temp.find(endChar);
		
		if(endCharIndex!=-1)
		{
			data->replace(startCharIndex, endCharIndex+1, replacement);
			
			startCharIndex=data->find(startChar);
		}
		else
		{
			startCharIndex = -1;
		}
		
		
	}
}

//----------------------------

int getNumberedPositionOfChar(string query, int pos, string * data)
{
	int numberedPosition=0;
	
	string temp = *data;
	
	for(int i=0;i<pos;i++)
	{
		int foundIndex=temp.find(query)+1;
		temp = temp.substr(foundIndex, temp.size()-foundIndex);
		numberedPosition+=foundIndex;
		//cout<<foundIndex<<endl;
	}
	
	return numberedPosition-1;
}

//----------------------------

void replaceMulti(string query, string replacement, string * data)
{
	//erase commas-------
	
	int queryIndex=data->find(query);
	
	
	while(queryIndex!=-1)
	{
		data->replace(queryIndex, query.size(), replacement);
		queryIndex=data->find(query);
	}
}

//----------------------------

int getNumOccurance(string query, string data)
{
	bool done = false;
	int numFound=0;
	while(!done)
	{
		
		int i = data.find(query);
		if(i!=-1)
		{
			numFound++;
			data = data.substr(i+query.size(),data.size()-i);
		}
		else
		{
			done=true;
		}
	}
	
	return numFound;
}

//----------------------------

void fileToStringVector(string file, vector<string>* strings)
{
	ifstream fin;
	fin.open( ofToDataPath(file).c_str() );
	
	while(fin!=NULL)
	{
		string str;
		getline(fin, str);
		strings->push_back(str);
	}
	
	fin.close();
}

//----------------------------

long isqrt (long x) /* Integer square root by Halleck's method, with Legalize's speedup */
{
	long   squaredbit, remainder, root;
	
	if (x<1) return 0;
	
	/* Load the binary constant 01 00 00 ... 00, where the number
	 * of zero bits to the right of the single one bit
	 * is even, and the one bit is as far left as is consistant
	 * with that condition.)
	 */
	squaredbit  = (long) ((((unsigned long) ~0L) >> 1) & 
						  ~(((unsigned long) ~0L) >> 2));
	/* This portable load replaces the loop that used to be 
	 * here, and was donated by  legalize@xmission.com 
	 */
	
	/* Form bits of the answer. */
	remainder = x;  root = 0;
	while (squaredbit > 0) {
		if (remainder >= (squaredbit | root)) {
			remainder -= (squaredbit | root);
			root >>= 1; root |= squaredbit;
		} else {
			root >>= 1;
		}
		squaredbit >>= 2; 
	}
	
	return root;
}


#endif