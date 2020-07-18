#include "commonOps.hpp"

std::string commonOps::passwordCalc(std::string pass, std::string salt, std::string rSalt)
{
    pass += salt;
    pass += rSalt;
    return commonOps::SHA3Wrapper(pass);
}

std::string commonOps::SHA3Wrapper(std::string str)
{
    std::string result;

    Poco::Crypto::DigestEngine eng("SHA3-256");
    eng.update(str);
    result = eng.digestToHex(eng.digest());
    Poco::toUpperInPlace(result);

    return result;
}

std::string commonOps::genRsalt(unsigned int tam)
{
    std::string rSalt = "";

    char ch;
    for(unsigned int i = 0; i < tam; ++i){
        do{
            ch = (char)rand();
        }while(ch < 33 || ch > 126);
        rSalt += ch;
    }

    return rSalt;
}

std::string commonOps::genAuthID(unsigned int tam)
{
    std::string resp = "";
    char val;
    for(unsigned int i = 0; i < tam; ++i){
        do{
            val = (char)rand();
        }while(!(val >= '0' && val <= '9') && !(val >= 'A' && val <= 'Z') && !(val >= 'a' && val <= 'z'));
        resp += val;
    }
    return resp;
}

Object::Ptr commonOps::erroOpJSON(unsigned int op, std::string erro)
{
    Poco::JSON::Object::Ptr respJSON = new Poco::JSON::Object;
    respJSON->set("status", Poco::NumberFormatter::format(op));
    respJSON->set("error", erro);
    return respJSON;
}

void commonOps::logMessage(std::string who, std::string what, Message::Priority howBad)
{
    Message msn(who, what, howBad);
    Application& app = Application::instance();
    app.logger().log(msn);
}