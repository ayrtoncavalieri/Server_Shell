#ifndef WEBSOCKREQHAND_H
#define WEBSOCKREQHAND_H

#include <iostream>
#include <cryptopp/sha3.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/hex.h>
#include "PocoInclude.hpp"

#define BUFSIZE 65536
#define MAXFRAMES 160

class WebSocketRequestHandler: public HTTPRequestHandler
{
    public: 
        void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response);

    private:
        std::string SHA3Wrapper(std::string str);
        std::string passwordCalc(std::string pass);
};

#endif