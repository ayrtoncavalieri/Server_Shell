#ifndef SERVEROPS_H
#define SERVEROPS_H

#include <iostream>
#include "PocoInclude.hpp"

class ServerOps
{
    public:
        ServerOps();
        ~ServerOps();
        std::string processReq(std::string req);
    private:
	std::string salt;
        std::string SHA3Wrapper(std::string str);
        std::string passwordCalc(std::string pass);
};

#endif
