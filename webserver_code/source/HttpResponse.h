

#ifndef _HTTPRESPONSE_H
#define _HTTPRESPONSE_H

#include "HttpMessage.h"

class HttpResponse : public HttpMesssage {

private:
 
    int status;

    std::string reason;

    void determineReasonStr();
   
    void determineStatusCode();

protected:
  
    virtual void init();

public:

    HttpResponse();
    HttpResponse(std::string sData);
    HttpResponse(byte *pData, unsigned int len);
    virtual ~HttpResponse();
     
    virtual byte* create();
    virtual bool parse();

    void setStatus(int scode){
	status = scode;
   	determineReasonStr();
    }

    std::string getReason(){
	return reason;
    }
   
};


#endif



