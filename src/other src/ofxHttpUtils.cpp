/*
    ofxHttpUtils v0.2
    Chris O'Shea, Art, Jesus, CJ

    Modified: 12th November
*/

#include "ofxHttpUtils.h"
#include "ofxHttpEvents.h"

// ----------------------------------------------------------------------
ofxHttpUtils::ofxHttpUtils(){
    timeoutSeconds = 40; // default timeout
    verbose = false;
    start();
}
// ----------------------------------------------------------------------
ofxHttpUtils::~ofxHttpUtils(){
    stop();
}
// ----------------------------------------------------------------------
void ofxHttpUtils::submitForm(ofxHttpForm form){
	doPostForm(form);
}
// ----------------------------------------------------------------------
void ofxHttpUtils::addForm(ofxHttpForm form){
    forms.push_back(form);
    if (isThreadRunning() == false){
        start();
    }
}
// ----------------------------------------------------------------------
void ofxHttpUtils::start() {
     if (isThreadRunning() == false){
        if(verbose) printf("starting http thread\n");
        startThread(true, verbose);
    }
}
// ----------------------------------------------------------------------
void ofxHttpUtils::stop() {
    stopThread();
}
// ----------------------------------------------------------------------
void ofxHttpUtils::threadedFunction(){

    // loop through this process whilst thread running
    while( isThreadRunning() == true )

    	if(forms.size()>0){
			ofxHttpForm form = forms.front();
			if(form.method==OFX_HTTP_POST){
				doPostForm(form);
				if(verbose) printf("ofxHttpUtils: (thread running) form submitted (post): %s\n", form.name.c_str());
			}else{
				string url = generateUrl(form);
				getUrl(url);
				if(verbose) printf("ofxHttpUtils: (thread running) form submitted (get): %s\n", form.name.c_str());
			}
			forms.erase( forms.begin() );
    	}else{
    		stop();
    	}

}

// ----------------------------------------------------------------------
string ofxHttpUtils::generateUrl(ofxHttpForm & form) {
    // url to send to
    string url = form.action;

    // do we have any form fields?
    if(form.formIds.size() > 0){
        url += "?";
        for(unsigned int i=0;i<form.formIds.size();i++){
            url += form.formIds[i] +"="+ form.formValues[i];
            if(i<form.formIds.size()-1)
                url += "&";
        }
    }
    return url;
}

// ----------------------------------------------------------------------
void ofxHttpUtils::doPostForm(ofxHttpForm & form){

    try{

        URI uri( form.action.c_str() );
        std::string path(uri.getPathAndQuery());
        if (path.empty()) path = "/";

        HTTPClientSession session(uri.getHost(), uri.getPort());
        HTTPRequest req(HTTPRequest::HTTP_POST, path, HTTPMessage::HTTP_1_1);

        session.setTimeout(Poco::Timespan(timeoutSeconds,0));

        // create the form data to send
        HTMLForm pocoForm(HTMLForm::ENCODING_URL);

        // form values
        for(unsigned int i=0; i<form.formIds.size(); i++){
            const std::string name = form.formIds[i].c_str();
            const std::string val = form.formValues[i].c_str();
            pocoForm.set(name, val);
        }

        pocoForm.prepareSubmit(req);

        pocoForm.write(session.sendRequest(req));
        HTTPResponse res;
        istream& rs = session.receiveResponse(res);

		ofxHttpResponse response=ofxHttpResponse(res, rs, form.action);

    	ofxHttpEvents.notifyNewResponse(response);


    }catch (Exception& exc){
        ofxHttpEvents.notifyNewError("time out ");
        if(verbose) std::cerr << exc.displayText() << std::endl;
    }

}

// ----------------------------------------------------------------------

//I've taken this function out for now whilst I make everything run ok in a thread
void ofxHttpUtils::getUrl(string url){
   try{
		URI uri(url.c_str());
		std::string path(uri.getPathAndQuery());
		if (path.empty()) path = "/";

		HTTPClientSession session(uri.getHost(), uri.getPort());
		HTTPRequest req(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
		session.sendRequest(req);

		session.setTimeout(Poco::Timespan(timeoutSeconds,0));

		HTTPResponse res;
		istream& rs = session.receiveResponse(res);

		ofxHttpResponse response=ofxHttpResponse(res,rs,url);

		ofxHttpEvents.notifyNewResponse(response);

	}catch (Exception& exc){
		if(verbose) std::cerr << exc.displayText() << std::endl;
		ofxHttpEvents.notifyNewError("time out ");
	}
}

void ofxHttpUtils::addUrl(string url){
	ofxHttpForm form;
	form.action=url;
	form.method=OFX_HTTP_GET;
    form.name=form.action;
	addForm(form);
	doPostForm(form);
	start();
}

ofxHttpUtils ofxHttpUtil;
ofxHttpEventManager ofxHttpEvents(&ofxHttpUtil);

