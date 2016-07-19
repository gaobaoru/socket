
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
































