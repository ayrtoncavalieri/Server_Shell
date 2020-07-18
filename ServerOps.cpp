#include "ServerOps.hpp"

ServerOps::ServerOps(): salt("S417_3*ampl3")
{

}

ServerOps::~ServerOps()
{

}

std::string ServerOps::processReq(std::string &req)
{
    Application& app = Application::instance();
    std::string respJSON;
    Poco::JSON::Parser p;
    unsigned int option;
    //Process data
    try{
        Poco::JSON::Object::Ptr reqJSON, procJSON;
        option = Poco::NumberParser::parseUnsigned(req.substr(0, 3));
        reqJSON = p.parse(req.substr(3, std::string::npos)).extract<Poco::JSON::Object::Ptr>();

        //Select option and do something.

        std::ostringstream streamg;
        procJSON->stringify(streamg, 0, -1);
        respJSON = streamg.str();
    }catch(Poco::JSON::JSONException &e){
        Message syntexc("ServerOps::processReq", "JSON with error", Message::PRIO_WARNING);
        app.logger().log(syntexc);
        respJSON = "{\"status\": \"" + req.substr(0, 3) + "\", \"error\":\"json_with_error\"}";
    }catch(Poco::SyntaxException &e){
        Message syntexc("ServerOps::processReq", "Number syntax error: " + req.substr(0, 3), Message::PRIO_WARNING);
        app.logger().log(syntexc);
        respJSON = "{\"status\": \"" + req.substr(0, 3) + "\", \"error\":\"wrong_format\"}";
    }catch(Poco::Exception &e){
        std::string reason = "Poco Exception: ";
        reason += e.className();
        reason += " -> ";
        reason += e.message();
        Message unk("ServerOps::processReq", "Unknown exception!", Message::PRIO_CRITICAL);
        app.logger().log(unk);
        respJSON = "{\"status\": \"" + req.substr(0, 3) + "\", \"error\":\"unknown_exception\"}";
    }catch( ... ){
        Message unk("ServerOps::processReq", "Unknown exception!", Message::PRIO_CRITICAL);
        app.logger().log(unk);
        respJSON = "{\"status\": \"" + req.substr(0, 3) + "\", \"error\":\"unknown_exception\"}";
    }
    
    return respJSON;
}
