/*
    ofxHttpUtils v0.2
    Chris O'Shea, Art, Jesus, CJ

    Modified: 12th November
*/

#ifndef _OFX_HTTP_UTILS
#define _OFX_HTTP_UTILS

#include "ofMain.h"

#include "ofxThread.h"

#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/StreamCopier.h"
#include "Poco/Path.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include "Poco/BasicEvent.h"
#include "Poco/Delegate.h"

#include <iostream>
//#include <queue.h>
#include <vector>
#include <istream.h>

using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTMLForm;
using Poco::Net::HTTPMessage;
using Poco::StreamCopier;
using Poco::Path;
using Poco::URI;
using Poco::Exception;
using Poco::BasicEvent;
using Poco::Delegate;

class ofxHttpListener;
class ofxHttpEventManager;

typedef Poco::Timestamp ofTimestamp;

#include "ofxHttpTypes.h"

struct ofxHttpResponse{
	ofxHttpResponse(HTTPResponse& pocoResponse, istream &bodyStream, string turl){
		status = pocoResponse.getStatus();
		timestamp = pocoResponse.getDate();
		reasonForStatus = pocoResponse.getReasonForStatus(pocoResponse.getStatus());
		contentType = pocoResponse.getContentType();

		StreamCopier::copyToString(bodyStream, responseBody);

		url = turl;

	}

	ofxHttpResponse(){}

	int status; 				// return code for the response ie: 200 = OK
	string reasonForStatus;		// text explaining the status
	string responseBody;		// the actual response
	string contentType;			// the mime type of the response
	ofTimestamp timestamp;		// time of the response
	string url;
};

class ofxHttpUtils : public ofxThread{

	public:

		ofxHttpUtils();
		~ofxHttpUtils();
		//-------------------------------
		// non blocking functions

		void addForm(ofxHttpForm form);
		void addUrl(string url);

		//-------------------------------
		// blocking functions
		void submitForm(ofxHttpForm form);
		void getUrl(string url);

        // other stuff-------------------
        int getQueueLength(){
            return forms.size();
        }
        void clearQueue(){
            forms.clear();
        }
        void setTimeoutSeconds(int t){
            timeoutSeconds = t;
        }
        void setVerbose(bool v){
            verbose = v;
        }
		//-------------------------------
		// threading stuff
		void threadedFunction();
    protected:
		void start();
        void stop();

		bool verbose;

		//--------------------------------
		// http utils
		string generateUrl(ofxHttpForm & form);
		void doPostForm(ofxHttpForm & form);

		vector <ofxHttpForm> forms;
		int timeoutSeconds;

};
#endif
