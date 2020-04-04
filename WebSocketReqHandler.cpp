#include "WebSocketReqHandler.hpp"

std::string WebSocketRequestHandler::SHA3Wrapper(std::string str)
{
    std::string result;

    Poco::Crypto::DigestEngine eng("SHA3-256");
    eng.update(str);
    result = eng.digestToHex(eng.digest());
    Poco::toUpperInPlace(result);
    
    return result;
}

std::string WebSocketRequestHandler::passwordCalc(std::string pass)
{
    std::string salt = "_Onj3TjOR*";
    pass += salt;
    return SHA3Wrapper(pass);
}

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
        while(frames <= MAXFRAMES){
            memset(buffer, '\0', BUFSIZE);
            n = ws.receiveFrame(buffer, BUFSIZE, flags);
            if((flags & WebSocket::FRAME_OP_BITMASK) == WebSocket::FRAME_OP_PING){
                app.logger().information(Poco::format("flags & WebSocket::FRAME_OP_BITMASK (flags=0x%x).", unsigned(flags & WebSocket::FRAME_OP_PING)));
                app.logger().information("A PING!");
                flags = (WebSocket::FRAME_FLAG_FIN | WebSocket::FRAME_OP_PONG);
                app.logger().information(Poco::format("flags = 0x%x", unsigned(flags)));
                n = 1;
                ws.sendFrame(buffer, 0, flags);
            }else if(buffer[0] == 0x04){
                break;
            }else{
                ++frames;
                incomeBuf += buffer;
            }
        }
        if(frames <= MAXFRAMES){
            //Processar as requisições
            if(respJSON.length() > BUFSIZE){
                int i;
                int len;
                std::string _send;
                for(i = 0; i < respJSON.length(); i += BUFSIZE){
                    _send = respJSON.substr(i, BUFSIZE);
                    flags = WebSocket::FRAME_FLAG_FIN | WebSocket::FRAME_OP_TEXT;
                    ws.sendFrame(_send.c_str(), _send.length(), flags);
                }
            }else{
                flags = WebSocket::FRAME_FLAG_FIN | WebSocket::FRAME_OP_TEXT;
                ws.sendFrame(respJSON.c_str(), respJSON.length(), flags);
            }
            ws.shutdown();
            n = ws.receiveFrame(buffer, BUFSIZE, flags);
            if((flags & WebSocket::FRAME_OP_BITMASK) != WebSocket::FRAME_OP_CLOSE){

            }else{
                ws.shutdown(1009, "WS_PAYLOAD_TOO_BIG");
            }
            app.logger().information("WebSocket connection closed.");
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
