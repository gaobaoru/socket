
#include "HttpMessage.h"
#include "HttpRequest.h"

HttpRequest::HttpRequest() : HttpMessage() {
    this->init();
}

HttpRequest::HttpRequest(std::string sData) : HttpMessage(sData) {
    this->init();
}

HttpRequest::HttpRequest(byte* pData, unsigned int len) : HttpMessage(pData, len) {
    this->init();
}

HttpRequest::~HttpRequest() {
}

void HttpRequest::init(){
    method = 0;
    requestUri = "";
}

int HttpRequest::methodStrToInt(std::string name) {
    if(name.empty() || (name.size() >= 10))
	return -1;
    int ret = -1;
    for(unsigned int i = 0; i < NUM_METHODS; i++){
	if(strcmp(requestMethodStr[i], name.c_str()) == 0){
	    ret = i;
   	    break;
	}
    }
    return ret;
}

std::string HttpRequest::methodIntToStr(unsigned int index){
    if(index >= NUM_METHODS)
	return "";
    
    return requestMethodStr[index];
}

// create HttpRequest messages

byte* HttpRequest::create(){
    clear();
    
    std::string mstr = "";
    mstr = methodIntToStr(method);
    if(mstr.empty()){
	printf("Could not create httpRequest, unknown method id: %d\n", method);
  	return NULL;
    }

    putLine(mstr + " " + requestUri + " " + version);

    putHeaders();

    if((data != NULL) && (dataLen > 0)){
	putBytes(data, dataLen);
    }

    byte* createRetData = new byte[size()];
    setReadPos(0);
    getBytes(createRetData, size());

    return createRetData;
} 

// parse HttpRequest messages

bool HttpRequest::parse(){
    std::string methodName = "";
    
    methodName = getStrElement();
    requestUri = getStrElement();
    version = getLine();

    method = methodStrToInt(methodName);
    if(method == -1){
	parseErrorStr = "Invalid Method: " + methodName;
 	return false;
    }

    parseHeaders();
   
    if((method != POST) && (method != PUT)){
	return true;
    }

    if(!parseBody())
	return false;

    return true;
}






