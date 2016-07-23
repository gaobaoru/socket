
#ifndef _HTTPREQUEST_H
#define _HTTPREQUEST_H

#include "HttpMessage.h"

class HttpRequest : public HttpMessage {

private:
    int method;

    std::string requestUri;

protected:

    virtual void init();

public:
    
    HttpRequest();
    HttpRequest(std::string sData);
    HttpRequest(byte *pData, unsigned int len);
    virtual ~HttpRequest();

    virtual byte *create();
    virtual bool parse();

    int methodStrToInt(std::string name);
    std::string methodIntToStr(unsigned int index);

    void setMethod(int m){
	method = m;
    }

    int getMethod() {
	return method;
    }

    void setRequestUri(std::string u) {
	requestUri = u;
    }

    std::string getRequestUri() {
	return requestUri;
    }
};


#endif












