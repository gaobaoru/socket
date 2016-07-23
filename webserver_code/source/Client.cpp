

#include "Client.h"

Client::Client(int fd, sockaddr_in addr){
    socketDesc = fd;
    clientAddr = addr;
}

Client::~Client(){
    clearSendQueue();
}


void Client::addToSendQueue(SendQueueItem* item) {
    sendQueue.push(item);
}

unsigned int Client::sendQueueSize(){
    return sendQueue.size();
}

SendQueueItem* Client::nextInSendQueue() {
    if(sendQueue.empty())
	return NULL;

    return sendQueue.front();
}

void Client::dequeueFromSendQueue(){
    SendQueueItem* item = nextInSendQueue();
    if(item != NULL){
	sendQueue.pop();
	delete item;
    }
}

void Client::clearSendQueue(){
    while(!sendQueue.empty()){
	delete sendQueue.front();
	sendQueue.pop();
    }
}




