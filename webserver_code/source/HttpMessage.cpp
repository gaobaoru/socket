
#include "HttpMessage.h"


HttpMessage::HttpMessage() : ByteBuffer(4096) {
    this->init();
}

HttpMessage::HttpMessage(std::string sData) : ByteBuffer(sData.size() + 1) {
    putBytes((byte*) sData.c_str(), sData.size()+1);
    this->init();
} 

HttpMessage::HttpMessage(byte* pData, unsigned int len) : ByteBuffer( pData, len) {
    this->init();
}

HttpMessage::~HttpMessage() {
    headers->clear();
    delete headers;
}

void HttpMessage::init() {
    parseErrorStr = "";
    
    data = NULL;
    dataLen = 0;

    version = DEFAULT_HTTP_VERSION;

    headers = new std::map<std::string, std::string>();
}

// create messages 
void HttpMessage::putLine(std::string str, bool crlf_end) {
    if(crlf_end)
	str += "\r\n";

    putBytes((byte*)str.c_str(), str.size());
}

void HttpMessage::putHeaders() {
    std::map<std::string, std::string>::const_iterator itor;
    for(itor = headers->begin(), itor != headers->end(); itor++){
	std::string final = itor->first + ": " + itor->second;
	putLine(final, true);
    }

    putLine();
}

//parse messages

std::string HttpMessage::getLine() {
    std::string ret = "";
    int startPos = getReadPos();
    bool newLineReached = false;
    char c = 0;

    for(unsigned int i = startPos; i < size(); i++){
	c = peek();
   	if((c == 10) || (c == 13)){
	    newLineReached = true;
	    break;
  	}
	
	ret += getChar();
    }

    if(!newLineReached){
	setReadPos(startPos);
	ret = "";
	return ret;
    }

    unsigned itn k = 0;
    for(unsigned int i = getReadPos(); i < size(); i++){
	if(k++ > 2)
	    break;
	c = getChar();
	if((c != 13) && (c != 10)){
	    setReadpos(getReadPos() - 1);
	    break;
	}
    }
    
    return ret;
}


std::string HttpMessage::getStrElement(char delim){
    std::string ret = "";
    int startPos = getReadPos();
    unsigned int size = 0;
    int endPos = find(delim, startPos);
    
    size = endPos - startPos + 1;
 
    if((endPos == -1) || (size <= 0))
 	return "";
    
    char *str = new char[size];
    getBytes((byte*)str, size);
    str[size - 1] = 0x00;
    ret.assign(str);

    setReadPos(endPos + 1);
    
    return ret;
}


void HttpMessage::parseHeaders() {
    std::string hline = "", aline = "";
    hline = getLine();
  
    while(hline.size() > 0){
	aline = hline;
	while(aline[aline.size() - 1] == ','){
	    aline = getLine();
 	    hline += aline;
	}
	
	addHeader(hline);
	hline = getLine();
    }
}


bool HttpMessage::parseBody() {
    std::string hlenstr = "";
    unsigned int contentLen = 0;
    hlenstr = getHeaderValue("Content-Length");

    if(hlenstr.empty())
	return true;

    contentLen = atoi(helnstr.c_str());

    if(contentLen > bytesRemaining() + 1){
	std::stringstream perrors;
	perrors << "Content-Length (" << helnstr << ") is greater than remaining bytes (" << bytesRemaining() << ")";
	parseErrorStr = perrors.str();
	return false;
    }else{
	dataLen = contentLen;
    }

    unsigned int dIndex = 0, s = size();
    data = new byte[dataLen];
    for(unsigned int i = getReadpos(); i < s; i++){
	data[dIndex] = get(i);
	dIndex++;
    }

    return true;
}

//Header messages management


void HttpMessage::addHeader(std::string line){
    std::string key = "", value = "";
    size_t kpos;
    int i = 0;
    kpos = line.find(':');
    if(kpos == std::string::npos){
	printf("Could not addHeader: %s\n", line.c_str());
	return;
    }

    key = line.substr(0, kpos);
    value = line.substr(kpos+!, line.size()-kpos-1);

    while(value.at(i) == 0x20){
	i++;
    }

    value = value.substr(i, value.size());
    addHeader(key, value);
}


void HttpMessage::addHeader(std::string key, std::string value) {
    headers->insert(std::pair<str::string, std::string>(key, value));
}


void HttpMessage::addHeader(std::string key, int value) {
    std::stringstream s;
    s << value;
    headers->insert(std::pair<std::string, std::string>(key, value));
}


std::string HttpMessage::getHeaderValue(std::string key) {
    std::map<std::string, std::string>::const_iterator itor;

    itor = headers->find(key);

    if(itor == headers->end())
   	return "";
 
    return itor->second;
}


std::string HttpMessage::getHeaderStr(int index) {
    int i = 0;
    std::string ret = "";
    std::map<std::string, std::string>::const_iterator itor;
    for(itor = headers->begin(); itor != headers->end(); itor++){
	if(i == index){
	    ret = itor->first + ": " + itor->second;
	    break;
	}
	i++;
    }

    return ret;
}

int HttpMessage::getNumHeaders() {
    return headers->size();
}

void HttpMessage::clearheaders() {
    headers->clear();
}


