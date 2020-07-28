#ifndef SERVEROPS_H
#define SERVEROPS_H

#include <iostream>
#include <exception>
#include "PocoInclude.hpp"
#include "commonOps.hpp"
#include <Poco/NumberParser.h>
#include <Poco/NumberFormatter.h>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Array.h>
#include <Poco/Data/DataException.h>

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
