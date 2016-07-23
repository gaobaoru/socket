

#ifndef _CLIENT_H
#define _CLIENT_H

#include <netinet/in.h>
#include <arpa/inet.h>
#include <queue>

#include "SendQueueItem.h"

typedef unsigned char byte;


class Client {

    int socketDesc;

    sockaddr_in clientAddr;

    std::queue<SendQueueItem*> sendQueue;

public:

    Client(int fd, sockaddr_in addr);
    ~Client();

    sockaddr_in getClientAddr() {
	return clientAddr;
    }        

    int getSocket(){
	return socketDesc;
    }

    char* getClientIP(){
	return inet_ntoa(clientAddr.sin_addr);
    }

    void addToSendQueue(SendQueueItem* item);

    unsigned int sendQueueSize();

    SendQueueItem* nextInSendQueue();

    void dequeueFromSendQueue();

    void clearSendQueue();
};


#endif








