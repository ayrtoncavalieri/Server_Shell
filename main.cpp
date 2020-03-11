#include <iostream>
#include <unistd.h>

#include "PocoInclude.hpp"
#include "WebSocketServer.hpp"

/*using namespace std;

int main(int argc, const char *argv[])
{
    try{
        Server srv(9980);
        cout << "OK\n";
        srv.startSrv();
        srv.supportIPv6() == true ? cout << "IPv6\n" : cout << "~IPv6\n";
    }catch(Poco::IllegalStateException e){
        cout << e.what();
    }
    
    return 0;
}*/

POCO_SERVER_MAIN(WebSocketServer);