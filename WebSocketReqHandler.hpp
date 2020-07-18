#ifndef WEBSOCKREQHAND_H
#define WEBSOCKREQHAND_H

#include <iostream>

#include "PocoInclude.hpp"
#include "ServerOps.hpp"

#define BUFSIZE 65536
#define MAXFRAMES 160
#define ONESEC 1000000

class WebSocketRequestHandler: public HTTPRequestHandler
{
    public: 
        void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response);
};

#endif