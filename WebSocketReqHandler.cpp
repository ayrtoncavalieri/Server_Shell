#include "WebSocketReqHandler.hpp"

void WebSocketRequestHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
{
Application& app = Application::instance();
    try
    {
        Timespan timeOut(0, 30 * ONESEC);
        WebSocket ws(request, response);
        ws.setReceiveTimeout(timeOut);
        ws.setSendTimeout(timeOut);
        ws.getBlocking() == true ? app.logger().information("blocking") : app.logger().information("~blocking");
        app.logger().information("WebSocket connection established.");
        char buffer[BUFSIZE + 1];
        int flags;
        int n;
        int frames;
        std::string incomeBuf;
        std::string respJSON;
        frames = 0;
        buffer[BUFSIZE] = '\0';
        //Receive frames
        while(frames <= MAXFRAMES){
            memset(buffer, '\0', BUFSIZE);
            n = ws.receiveFrame(buffer, BUFSIZE, flags);
            if((flags & WebSocket::FRAME_OP_BITMASK) == WebSocket::FRAME_OP_PING){ // Process PINGS
                app.logger().information(Poco::format("flags & WebSocket::FRAME_OP_BITMASK (flags=0x%x).", unsigned(flags & WebSocket::FRAME_OP_PING)));
                app.logger().information("A PING!");
                flags = (WebSocket::FRAME_FLAG_FIN | WebSocket::FRAME_OP_PONG);
                app.logger().information(Poco::format("flags = 0x%x", unsigned(flags)));
                n = 1;
                ws.sendFrame(buffer, 0, flags);
            }else if(buffer[0] == 0x04){ // Received EOT and ending income
                break;
            }else{ // Adding income to buffer
                ++frames;
                incomeBuf += buffer;
            }
        }
        if(frames <= MAXFRAMES){
            //Process requisitions
            ServerOps srv;
            respJSON = srv.processReq(incomeBuf);

            if(respJSON.length() > BUFSIZE){ //Sending a big response
                int i;
                std::string _send;
                for(i = 0; i < respJSON.length(); i += BUFSIZE){
                    _send = respJSON.substr(i, BUFSIZE);
                    flags = WebSocket::FRAME_FLAG_FIN | WebSocket::FRAME_OP_TEXT;
                    ws.sendFrame(_send.c_str(), _send.length(), flags);
                }
            }else{ //Sending a small response
                flags = WebSocket::FRAME_FLAG_FIN | WebSocket::FRAME_OP_TEXT;
                ws.sendFrame(respJSON.c_str(), respJSON.length(), flags);
            }
            ws.shutdown();
            n = ws.receiveFrame(buffer, BUFSIZE, flags);
            if((flags & WebSocket::FRAME_OP_BITMASK) != WebSocket::FRAME_OP_CLOSE){

            }
            app.logger().information("WebSocket connection closed.");
        }else{ //Warning sender that the payload is too big
            ws.shutdown(1009, "WS_PAYLOAD_TOO_BIG");
        }
    }catch (WebSocketException& exc){
        app.logger().log(exc);
        switch (exc.code())
        {
        case WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION:
            response.set("Sec-WebSocket-Version", WebSocket::WEBSOCKET_VERSION);
            // fallthrough
        case WebSocket::WS_ERR_NO_HANDSHAKE:
        case WebSocket::WS_ERR_HANDSHAKE_NO_VERSION:
        case WebSocket::WS_ERR_HANDSHAKE_NO_KEY:
            response.setStatusAndReason(HTTPResponse::HTTP_BAD_REQUEST);
            response.setContentLength(0);
            response.send();
            break;
        }
    }catch (TimeoutException& e){
        app.logger().log(e);
        app.logger().information("Timeout exception.");
    }catch(NetException &e){
        app.logger().log(e);
        app.logger().information("Network exception.");
    }
}
