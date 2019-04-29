/*
    ofxHttpUtils v0.2
    Chris O'Shea, Art, Jesus, CJ

    Modified: 12th November
*/

#ifndef OFX_HTTP_EVENTS
#define OFX_HTTP_EVENTS

#include "ofxHttpUtils.h"

class ofxHttpEventListener{
public:
	virtual void newResponse( ofxHttpResponse & response ){};
	virtual void newError(string & error) {};
	void newResponse(const void * sender, ofxHttpResponse & response){
		newResponse(response);
	}
    void newError(const void * sender, string & error){
        newError(error);
    }
};

class ofxHttpEventManager{
    public:
        ofxHttpEventManager(ofxHttpUtils * sender){
            this->sender=sender;
        }
        void addListener(ofxHttpEventListener * listener){
            responseEvent += Poco::Delegate<ofxHttpEventListener,ofxHttpResponse>(listener,&ofxHttpEventListener::newResponse);
            errorEvent    += Poco::Delegate<ofxHttpEventListener,string>(listener,&ofxHttpEventListener::newError);
        }

        void removeListener(ofxHttpEventListener * listener){
            responseEvent -= Poco::Delegate<ofxHttpEventListener,ofxHttpResponse>(listener,&ofxHttpEventListener::newResponse);
            errorEvent    -= Poco::Delegate<ofxHttpEventListener,string>(listener,&ofxHttpEventListener::newError);
        }

        void notifyNewResponse(ofxHttpResponse response){
            responseEvent.notify(sender,response);
        }
        void notifyNewError(string error){
			errorEvent.notify(sender,error);
		}
    protected:
        Poco::BasicEvent <ofxHttpResponse> responseEvent;
        Poco::BasicEvent<string>           errorEvent;
        ofxHttpUtils * sender;
};

extern ofxHttpUtils ofxHttpUtil;
extern ofxHttpEventManager ofxHttpEvents;
#endif /* OFXHTTPEVENTS_H_ */
