

#include <iostream>
#include <string>
#include <signal.h>

#include "HttpServer.h"

static HttpServer* svr;

void handleSigPipe(int snum){
    return;
}

void handleTermSig(int snum){
    svr->canRun = false;
}

int main(){

    signal(SIGPIPE, handleSigPipe);

    signal(SIGABRT, &handleTermSig);
    signal(SIGINT, &handleTermSig);
    signal(SIGTERM, &handleTermSig);

    svr = new HttpServer();
    svr->start(8080);

    svr->process();

    svr->stop();
    delete svr;
    
    return 0;
}
