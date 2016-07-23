

#include "HttpServer.h"

HttpServer::HttpServer(){
    canRun = false;
    listenSocket = INVALID_SOCKET;

    ResourceHost* resHost = new ResourceHost("./ResourceHost");
    hostList.push_back(resHost);

    vhosts.insert(std::pair<std::string, ResourceHost*>("localhost:8080", resHost));
    vhosts.insert(std::pair<std::string, ResourceHost*>("127.0.0.1:8080", resHost));
}

HttpServer::~HttpServer(){
    while(!hostList.empty()){
	ResourceHost* resHost = hostList.back();
   	delete resHost;
	hostList.pop_back();
    }
	vhosts.clear();
}


bool HttpServer::start(int port) {
    canRun = false;
    listenPort = port;
  
    listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listenSocket == INVALID_SOCKET){
	std::cout<< "Could not create socket!" << std::endl;
	return false;
    }

    fcntl(listenSocket, F_SETFL, O_NONBLOCK);

    memset(&serverAddr, 0, sizeof(struct sockaddr_in));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(listenPort);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if(bind(listenSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != 0) {
	std::cout << "Failed to bind to the address!" << std::endl;
	return false;
    }

    if(listen(listenSocket, SOMAXCONN) != 0){
	std::cout<< "Failed to put the socket in a listening state" << std::endl;
	return false;
    }

    kqfd = kqueue();
    if(kqfd == -1){
	std::cout << "Could not create the kernel event queue!" << std::endl;
	return false;
    }

    updateEvent(listenSocket, EVFILT_READ, EV_ADD, 0, 0, NULL);

    canRun = true;
    std::cout << "Server ready. Listending on port " << listenPort << "..." << std::endl;
    return true;
}


void HttpServer::stop() {

    canRun = false;
    if(listenSocket != INVALID_SOCKET) {
	for(auto x : clientMap){
	    disconnectClient(x.second, false);
 	}

	clientMap.clear();
 
 	updateEvent(listenSocket, EVFILT_READ, EV_DELETE, 0, 0, NULL);

	shutdown(listenSocket, SHUT_RDWR);
	close(listenSocket);
	listenSocket = INVALID_SOCKET;
    }

    if(kqfd != -1){
	close(kqfd);
	kqfd = -1;
    }

    std::cout << "Server shutdown!" << std::endl;
}


void HttpServer::updateEvent(int ident, short filter, u_short flags, u_int fflags, int data, void* udata){
    struct kevent kev;
    EV_SET(&kev, ident, filter, flags, fflags, data, udata);
    kevent(kqfd, &kev, 1, NULL, 0, NULL);
}


// the main loop

void HttpServer::process() {
    int nev = 0;
    Client* cl = NULL;

    struct kevent read_kev, write_kev;

    while(canRun){
	
 	nev = kevent(kqfd, NULL, 0, evList, QUEUE_SIZE, &kqTimeout);

	if(nev <= 0)
	   continue;
	for(int i = 0; i < nev; i++){
	   if(evList[i].ident == (unsigned int)listenSocket) {
		acceptConnection();
 	   }else{
		
		cl = getClient(evList[i].ident);
		if(cl == NULL){
		    std::cout << "Could not find client" << std::endl;
		    continue;
		}
		
		if(evList[i].flags & EV_EOF) {
		    disconnectClient(cl);
		    continue;
		}

 		memset(&read_kev, 0, sizeof(struct kevent));
		memset(&write_kev, 0, sizeof(struct kevent));

		if(evList[i].filter == EVFILT_READ) {
		    readClient(cl, evList[i].data);

		    updateEvent(evList[i].ident, EVFILT_READ, EV_DISABLE, 0, 0, NULL);
		    updateEvent(evList[i].ident, EVFILT_WRITE, EV_ENABLE, 0, 0, NULL);
		} else if(evList[i].filter == EVFILT_WRITE){
		    
		    if(!writeClient(cl, evList[i].data)){
			updateEvent(evList[i].ident, EVFILT_READ, EV_ENABLE, 0, 0, NULL);
			updateEvent(evList[i].ident, EVFILT_WRITE, EV_DISABLE, 0, 0, NULL);	
    		    }
		}
	   }
 	}
    }
}


// client connection functions

void HttpServer::acceptConnection() {
    sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    int clfd = INVALID_SOCKET;

    clfd = accept(listenSocket, (sockaddr*)&clientAddr, (socklen_t*)&clientAddrLen);
    if(clfd == INVALID_SOCKET)
	return;

    fcntl(clfd, F_SETFL, O_NONBLOCK);

    Client* cl = new Client(clfd, clientAddr);

    updateEvent(clfd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
    updateEvent(clfd, EVFILT_WRITE, EV_ADD | EV_DISABLE, 0, 0, NULL);

    clientMap.insert(std::pair<int, Client*>(clfd, cl));

    std::cout << "[" << cl->getClientIP() << "} connected" << std::endl;
    
}


Client* HttpServer::getClient(int clfd){
    std::unordered_map<int, Client*>::const_iterator itor;
    itor = clientMap.find(clfd);

    if(itor == clientMap.end())
	return NULL;

    return itor->second;
}



void HttpServer::disconnectClient(Client *cl, bool mapErase) {
    if(cl == NULL)
	return;

    std::cout << "[" << cl->getClientIP() << "] disconnected" << std::endl;

    updateEvent(cl->getSocket(), EVFILT_READ, EV_DELETE, 0, 0, NULL);
    updateEvent(cl->getSocket(), EVFILT_WRITE, EV_DELETE, 0, 0, NULL);

    close(cl->getSocket());

    if(mapErase)
	clientMap.erase(cl->getSocket());

    delete cl;
}


void HttpServer::readClient(Client *cl, int data_len) {
    if(cl == NULL)
	return;
    if(data_len <= 0)
	data_len = 1400;

    HttpRequest* req;
    char* pData = new char[data_len];

    int flags = 0;
    ssize_t lenRecv = recv(cl->getSocket(), pData, data_len, flags);

    if(lenRecv == 0){
	std::cout<< "[" << cl->getSocket() << "] has opted to close the connection" <<std::endl;
  	disconnectClient(cl);
    } else if(lenRecv < 0) {
	disconnectClient(cl);
    } else {
	req = new HttpRequest((byte*)pData, lenRecv);
	handleRequest(cl, req);
	delete req;
    }

    delete [] pData;
}


bool HttpServer::writeClient(Client* cl, int avail_bytes){
    if(cl == NULL)
	return false;

    int actual_sent = 0;
    int attempt_sent = 0;
    int remaining = 0;
    bool disconnect = false;
    byte* pData = NULL;

    if(avail_bytes > 1400)
	avail_bytes = 1400;
    else if(avail_bytes == 0)
	avail_bytes = 64;

    SendQueueItem* item = cl->nextInSendQueue();
    if(item == NULL)
	return false;
    pData = item->getData();
    remaining = item->getSize() - item->getOffset();
    disconnect = item->getDisconnect();

    if(avail_bytes > remaining)
	attempt_sent = remaining;
    else
	attempt_sent = avail_bytes;

    actual_sent = send(cl->getSocket(), pData+(item->getOffset()), attempt_sent, 0);
    if(actual_sent >= 0)
	item->setOffset(item->getOffset() + actual_sent);
    else
	disconnect = true;

    if(item->getOffset() >= item->getSize())
	cl->dequeueFromSendQueue();

    if(disconnect){
	disconnectClient(cl);
	return false;
    }

    return true;
}


// handle the request

void HttpServer::handleRequest(Client* cl, HttpRequest* req) {
    if(!req->parse()) {
	std::cout << "[" << cl->getClientIP() << "] There was an error processing the request of type: " << req->methodIntToStr(req->getMethod()) << std::endl;
	std::cout << req->getParseError() << std::endl;
	sendStatusResponse(cl, Status(BAD_REQUEST), req->getParseError());
	return;
    }

    std::cout << "[" << cl->getClientIP() << "]" << req->methodIntToStr(req->getMethod()) << " " << req->getRequestUri() << std::endl;

    ResourceHost* resHost = NULL;
    std::string host = "";

    if(req->getVersion().compare(HTTP_VERSION_11) == 0){
	host = req->getHeaderValue("Host");
	std::unordered_map<std::string, ResourceHost*>::const_iterator it = vhosts.find(host);

   	if(it != vhosts.end())
	    resHost = it->second;
    }else {
	if(hostList.size() > 0)
	     resHost = hostList[0];
    }

    if(resHost == NULL){
	sendStatusResponse(cl, Status(BAD_REQUEST), "Invalid/No Host specified: " + host);
	return;
    }

    switch(req->getMethod()){
	case Method(HEAD):
	case Method(GET):
	    handleGet(cl, req, resHost);
	    break;
	case Method(OPTIONS):
	    handleOptions(cl, req);
 	    break;
  	default:
	    std::cout << "[" << cl->getClientIP() << "] Could not handle or determine request of type" << req->methodIntToStr(req->getMethod()) << std::endl;
	    sendStatusResponse(cl, Status(NOT_IMPLEMENTED));
	    break;
    }

}


void HttpServer::handleGet(Client* cl, HttpRequest* req, ResourceHost* resHost){
    std::string uri = req->getRequestUri();
    Resource* r = resHost->getResource(uri);

    if(r != NULL){
	HttpResponse* resp = new HttpResponse();
	resp->setStatus(Status(OK));
	resp->addHeader("Content-Type", r->getMimeType());
	resp->addHeader("Content-Length",r->getSize());

  	if(req->getMethod() == Method(GET))
	    req->setData(r->getData(), r->getSize());

	bool dc = false;

	if(req->getVersion().compare(HTTP_VERSION_10) == 0)
	    dc = true;

  	std::string connection_val = req->getHeaderValue("Connection");
	if(connection_val.compare("close") == 0)
	    dc = true;

	sendResponse(cl, resp, dc);
	delete resp;
	delete r; 
    } else {
	sendStatusResponse(cl, Status(NOT_FOUND));
    }
}

void HttpServer::handleOptions(Client* cl, HttpRequest* req){
    std::string allow = "HEAD, GET, OPTIONS";

    HttpResponse* resp = new HttpResponse();
    resp->setStatus(Status(OK));
    resp->addHeader("Allow", allow.c_str());
    resp->addHeader("Content-Length", "0");

    sendResponse(cl, resp, true);
    delete resp;
}

//send Status code to the client

void HttpServer::sendStatusResponse(Client* cl, int status, std::string msg) {
    HttpResponse* resp = new HttpResponse();
    resp->setStatus(Status(status));
    
    std::string body = resp->getReason() + ": " + msg;
    unsigned int slen = body.length();
    char* sdata = new char[slen];
    strncpy(sdata, body.c_str(), slen);

    resp->addHeader("Content-Type", "Text/plain");
    resp->addHeader("Content-Length", slen);
    resp->setData((byte*)sdata, slen);

    sendResponse(cl, resp, true);

    delete resp;
}

void HttpServer::sendResponse(Client* cl, HttpResponse* resp, bool disconnect) {
	
	resp->addHeader("Server", "godfreyserver/1.0");
	
	std::string tstr;
	char tbuf[36];
	time_t rawtime;
	struct tm* ptm;
	time(&rawtime);
	ptm = gmtime(&rawtime);
	
	strftime(tbuf, 36, "%a, %d %b %Y %H:%M:%S GMT", ptm);
	tstr = tbuf;
	resp->addHeader("Date", tstr);
	
	// 是否需要在头部中添加断开连接的信息
	if(disconnect)
		resp->addHeader("Connection", "close");
	
	// 创建响应数据
	byte* pData = resp->create();

	// 将数据添加到客户端的发送队列
	cl->addToSendQueue(new SendQueueItem(pData, resp->size(), disconnect));
}






