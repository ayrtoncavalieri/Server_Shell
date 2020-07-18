#ifndef COMMOPS_H
#define COMMOPS_H

#include "PocoInclude.hpp"
#include <cstdlib>
#include <Poco/NumberFormatter.h>
#include <Poco/JSON/Object.h>
#include <Poco/Crypto/DigestEngine.h>

using namespace Poco::JSON;

class commonOps{
    public:
        static std::string passwordCalc(std::string pass, std::string salt, std::string rSalt = "");
        static std::string genRsalt(unsigned int tam);
        static std::string genAuthID(unsigned int tam);
        static Object::Ptr erroOpJSON(unsigned int op, std::string erro);
        static void logMessage(std::string who, std::string what, Message::Priority howBad);
    private:
        static std::string SHA3Wrapper(std::string str);

};

#endif