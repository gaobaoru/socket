
#ifndef _SENDQUEUEITEM_H
#define _SENDQUEUEITEM_H

#include <cstdlib>

typedef unsigned char byte;

class SendQueueItem {

private:
    
    byte* sendData;

    unsigned int sendSize;

    unsigned int sendOffset;
 
    bool disconnect;

public:
    
    SendQueueItem(byte* data, unsigned int size, bool dc) {
	sendData = data;
  	sendSize = size;
	sendOffset = 0;
	disconnect = dc;
    }

    ~SendQueueItem() {
	if(sendData != NULL){
	    delete [] sendData;
	    sendData = NULL; 
	}
    }

    void setOffset(unsigned int off){
	sendOffset = off;
    }

    byte* getData(){
	return sendData;
    }

    unsigned int getSize(){
	return sendSize;
    }

    unsigned int getOffset() {
	return sendOffset;
    }

    bool getDisconnect() {
	return disconnect;
    }
};


#endif


