#include "ServerOps.hpp"

ServerOps::ServerOps(): salt("_Onj3TjOR*")
{

}

ServerOps::~ServerOps()
{

}

std::string ServerOps::processReq(std::string req)
{
    std::string respJSON;
    //Process data
    
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
    pass += salt;
    return SHA3Wrapper(pass);
}
