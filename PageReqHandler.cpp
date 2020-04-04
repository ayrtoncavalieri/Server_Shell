#include "PageReqHandler.hpp"

void PageRequestHandler::handleRequest(HTTPServerRequest &request, HTTPServerResponse &response)
{
	//response.setStatusAndReason(response.HTTP_BAD_REQUEST);
    response.setChunkedTransferEncoding(true);
		response.setContentType("text/html");
		std::ostream& ostr = response.send();
		ostr << "<html>";
		ostr << "<head>";
		ostr << "<title>WebSocketServer</title>";
		ostr << "</head>";
		ostr << "<body onload=\"redirect()\">";
		ostr << "  <p></p>";
		ostr << "<script type=\"text/javascript\">";
		ostr << "function redirect(){";
		ostr << "window.location.replace(\"http://www.aplikoj.com/vue\");";
		ostr << "}";
		ostr << "</script>";
		ostr << "</body>";
		ostr << "</html>";
}
