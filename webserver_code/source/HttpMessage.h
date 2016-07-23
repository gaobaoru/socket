
#ifndef _HTTPMESSAGE_H
#define _HTTPMESSAGE_H

#include <map>
#include <string>
#include <sstream>

#include "ByteBuffer.h"

#define HTTP_VERSION_10 "HTTP/1.0"
#define HTTP_VERSION_11 "HTTP/1.1"
#define DEFAULT_HTTP_VERSION "HTTP/1.1"

#define NUM_METHODS 9

enum Method{
    HEAD = 0,
    GET = 1,
    POST = 2,
    PUT = 3,
    DEL = 4,
    TRACE = 5,
    OPTIONS = 6,
    CONNECT = 7,
    PATCH = 8
};


const static char* const requestMethodStr[NUM_METHODS] = {
    "HEAD",  // 0
    "GET",   // 1
    "POST",  // 2
    "PUT",   // 3
    "DEL",   // 4
    "TRACE", // 5
    "OPTIONS",// 6
    "CONNECT",// 7
    "PATCH"   // 8
};

enum Status{
    // 1xx
    CONTINUE = 100,
    //2xx
    OK = 200,
    //4xx
    BAD_REQUEST = 400,
    NOT_FOUND = 404,
    //5xx
    SERVER_ERROR = 500,
    NOT_IMPLEMENTED = 501
};


class HttpMessage : public ByteBuffer {
private:

    std::map<std::string, std::string> *headers;

protected:
 
    std::string parseErrorStr;

    std::string version;

    byte* data;
 
    unsigned int dataLen;

    virtual void init();

public:

    HttpMessage();

    HttpMessage(std::string sData);

    HttpMessage(byte* pData, unsigned int len);

    virtual ~HttpMessage();

    virtual byte* create() = 0;

    virtual bool parse() = 0;

    void putLine(std::string str = "", bool crif_end = true);
    void putHeaders();

    std::string getLine();
    std::string getStrElement(char delim = 0x20);
    void parseHeaders();
    bool parseBody();

    void addHeader(std::string line);
    void addHeader(std::string key, std::string value);
    void addHeader(std::string key, int value);
    std::string getHeaderValue(std::string key);
    std::string getHeaderStr(int index);
    int getNumHeaders();
    void clearHeaders();

    std::string getParseError() {
	return parseErrorStr;
    }

    void setVersion(std::string v) {
	version = v;
    }

    std::string getVersion() {
	return version;
    }

    void setData(byte* d, unsigned int len){
	data = d;
  	dataLen = len;
    }

    byte* getData() {
	return data;
    }
 
    unsigned int getDataLength() {
	return dataLen;
    }

};


#endif

