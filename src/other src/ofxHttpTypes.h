/*
    ofxHttpUtils v0.2
    Chris O'Shea, Art, Jesus, CJ

    Modified: 12th November
*/

#ifndef OFX_HTTP_TYPES
#define OFX_HTTP_TYPES

#define OFX_HTTP_GET  0
#define OFX_HTTP_POST 1

struct ofxHttpForm{
	vector <string> formIds;
	vector <string> formValues;

	// ----------------------------------------------------------------------
	void addFormField(string id, string value){
	    formIds.push_back( id );
	    formValues.push_back( value );
	}
	// ----------------------------------------------------------------------
	void clearFormFields(){
	    formIds.clear();
	    formValues.clear();
	}
	int method;
	string action;
	string name;
};

#endif
