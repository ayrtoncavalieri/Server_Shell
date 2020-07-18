#include "ReqHandlerFactory.hpp"

HTTPRequestHandler* RequestHandlerFactory::createRequestHandler(const HTTPServerRequest &request)
{
    Application& app = Application::instance();
    Message msn("ReqHandlerFactory", "Request from "
        + request.clientAddress().toString()
        + ": "
        + request.getMethod()
        + " "
        + request.getURI()
        + " "
        + request.getVersion(), Message::PRIO_INFORMATION);
    app.logger().log(msn);
#ifdef TRACE
    for (HTTPServerRequest::ConstIterator it = request.begin(); it != request.end(); ++it)
    {
        msn.setSource("ReqHandlerFactory");
        msn.setText(it->first + ": " + it->second);
        msn.setPriority(Poco::Message::PRIO_DEBUG);
        app.logger().log(msn);
    }
#endif
    if(request.find("Upgrade") != request.end() && Poco::icompare(request["Upgrade"], "websocket") == 0
    && request.find("sec-websocket-protocol") != request.end() && Poco::icompare(request["sec-websocket-protocol"], "PDRAUM") == 0)
        return new WebSocketRequestHandler;
    else
        return new PageRequestHandler;
}
