#include "ServerOps.hpp"

ServerOps::ServerOps()
{

}

ServerOps::~ServerOps()
{
    
}

std::string ServerOps::processReq(std::string req)
{
    std::string respJSON;

    return respJSON;
}

std::string ServerOps::SHA3Wrapper(std::string str)
{
    std::string result;

    Poco::Crypto::DigestEngine eng("SHA3-256");
    eng.update(str);
    result = eng.digestToHex(eng.digest());
    Poco::toUpperInPlace(result);
    
    return result;
}

std::string ServerOps::passwordCalc(std::string pass)
{
    std::string salt = "_Onj3TjOR*";
    pass += salt;
    return SHA3Wrapper(pass);
}