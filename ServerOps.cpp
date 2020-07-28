#include "ServerOps.hpp"

ServerOps::ServerOps(): salt("S417_3*ampl3")
{

}

ServerOps::~ServerOps()
{

}

std::string ServerOps::processReq(std::string &req)
{
    std::string respJSON;
    Poco::JSON::Object::Ptr reqJSON, procJSON;  
    Poco::JSON::Parser p;
    unsigned int option = 0;
    //Process data
    try{
        reqJSON = p.parse(req.substr(3, std::string::npos)).extract<Poco::JSON::Object::Ptr>();
        option = reqJSON->getValue<unsigned int>("option");

        //Select option and do something.

    }catch(Poco::InvalidAccessException &e){
        commonOps::logMessage("ServerOps::processReq", "JSON InvalidAccess", Message::PRIO_WARNING);
        procJSON = commonOps::erroOpJSON(option, "json_with_error");
    }catch(Poco::RangeException &e){
        commonOps::logMessage("ServerOps::processReq", "JSON RangeException", Message::PRIO_WARNING);
        procJSON = commonOps::erroOpJSON(option, "json_with_error");
    }catch(Poco::JSON::JSONException &e){
        commonOps::logMessage("ServerOps::processReq", "JSON with error", Message::PRIO_WARNING);
        procJSON = commonOps::erroOpJSON(option, "json_with_error");
    }catch(Poco::Data::DataException &e){
        std::string reason = "Poco DataException: ";
        reason += e.className();
        reason += " -> ";
        reason += e.message();
        commonOps::logMessage("ServerOps::processReq", reason, Message::PRIO_CRITICAL);
        procJSON = commonOps::erroOpJSON(option, "data_exception");
    }catch(Poco::Exception &e){
        std::string reason = "Poco Exception: ";
        reason += e.className();
        reason += " -> ";
        reason += e.message();
        commonOps::logMessage("ServerOps::processReq", reason, Message::PRIO_CRITICAL);
        procJSON = commonOps::erroOpJSON(option, "server_exception");
    }catch(std::exception &e){
        std::string reason = "std::exception";
        reason += " -> ";
        reason += e.what();
        commonOps::logMessage("ServerOps::processReq", reason, Message::PRIO_CRITICAL);
        procJSON = commonOps::erroOpJSON(option, "std_exception");
    }catch( ... ){
        commonOps::logMessage("ServerOps::processReq", "Unknown exception!", Message::PRIO_CRITICAL);
        procJSON = commonOps::erroOpJSON(option, "unknown_exception");
    }
    
    std::ostringstream streamg;
    procJSON->stringify(streamg, 0, -1);
    respJSON = streamg.str();
    return respJSON;
}
