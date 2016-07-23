

#ifndef _HTTPSERVER_H
#define _HTTPSERVER_H

#include <unordered_map>
#include <vector>
#include <string>

#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>


#ifdef __linux__
    #include <kqueue/sys/event.h>
#else
    #include <sys/event.h>
#endif

#include "Client.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "ResourceHost.h"

#define INVALID_SOCKET -1

#define QUEUE_SIZE 1024

class HttpServer{

    int listenPort;

    int listenSocket;

    struct sockaddr_in serverAddr;

    struct timespec kqTimeout = {2, 0};

    int kqfd;

    struct kevent evList[QUEUE_SIZE];

    std::unordered_map<int, Client*> clientMap;

    std::vector<ResourceHost*> hostList;

    std::unordered_map<std::string, ResourceHost*> vhosts;

    //client connection

    void updateEvent(int ident, short filter, u_short flags, u_int fflags, int data, void *udata);
    void acceptConnection();
    Client* getClient(int clfd);
    void disconnectClient(Client* cl, bool mapErase = true);
    void readClient(Client* cl, int data_len);
    bool writeClient(Client* cl, int avail_bytes);

    //client request

    void handleRequest(Client* cl, HttpRequest* req);
    void handleGet(Client* cl, HttpRequest* req, ResourceHost* resHost);
    void handleOptions(Client* cl, HttpRequest* req);

    // send response to client

    void sendStatusResponse(Client* cl, int status, std::string msg = "");
    void sendResponse(Client* cl, HttpResponse* resp, bool disconnect);

public:
  
    bool canRun;

    HttpServer();
    ~HttpServer();
    
    bool start(int port);
    void stop();

    void process();
  
};

#endif





