

#include "HttpMessage.h"
#include "HttpResponse.h"

HttpResponse::HttpResponse() : HttpMessage() {
    this->init();
}

HttpResponse::HttpResponse(std::string sData) : HttpMessage(sData) {
    this->init();
}

HttpResponse::HttpResponse(byte* pData, unsigned int len) : HttpMessage(pData, len) {
    this->init();
}


HttpResponse::~HttpResponse() {
}

void HttpResponse::init() {
    status = 0;
    reason = "";
}

void HttpResponse::determineStatusCode() {
    if(reason.find("Continue") != std::string::npos){
	status = Status(CONTINUE);
    }else if(reason.find("OK") != std::string::npos){
	status = Status(OK);
    }else if(reason.find("Bad Request") != std::string::npos){
	status = Status(BAD_REQUEST);
    }else if(reason.find("Not Found") != std::string::npos){
	status = Status(NOT_FOUND);
    }else if(reason.find("Server Error") != std::string::npos){
	status = Status(SERVER_ERROR);
    }else if(reason.find("Not Implemented") != std::string::npos){
	status = Status(NOT_IMPLEMENTED);
    }else{
    }
}


void HttpResponse::determineReasonStr(){
    switch(status){
	case Status(CONTINUE):
	    reason = "Continue";
	    break;
	case Status(OK):
	    reason = "OK";
	    break;
	case Status(BAD_REQUEST):
	    reason = "Bad Request";
	    break;
	case Status(NOT_FOUND):
	    reason = "Not Found";
	    break;
	case Status(SERVER_ERROR):
	    reason = "Server Error";
 	    break;
    	case Status(NOT_IMPLEMENTED):
	    reason = "Not Implemented";
   	    break;
	default:
	    break;
    }
}


byte* HttpResponse::create() {
    clear();

    std::stringstream sline;
    sline << version << " " << status << " " << reason;
    putLine(sline.str());

    putHeaders();

    if((data != NULL) && dataLen > 0) {
        putBytes(data, dataLen);
    }

    byte* createRetData = new byte[size()];
    setReadPos(0);
    getBytes(createRetData, size());

    return createRetData;
}

bool HttpResponse::parse(){
    std::string statusstr;

    version = getStrElement();
    statusstr = getStrElement();
    determineStatusCode();
    reason = getLine();

    parseHeaders();

    if(!parseBody())
   	return false;

    return true;
}



















