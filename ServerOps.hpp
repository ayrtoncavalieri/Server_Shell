#ifndef SERVEROPS_H
#define SERVEROPS_H

#include <iostream>
#include "PocoInclude.hpp"
#include <Poco/NumberParser.h>
#include <Poco/NumberFormatter.h>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Array.h>

class ServerOps
{
    public:
        ServerOps();
        ~ServerOps();
        std::string processReq(std::string &req);
    private:
        std::string salt;
};

#endif
