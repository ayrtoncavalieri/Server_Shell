#include "WebSocketServer.hpp"

WebSocketServer::WebSocketServer() :_helpRequested(false)
{

}

WebSocketServer::~WebSocketServer()
{

}

void WebSocketServer::initialize(Application &self)
{
    loadConfiguration(); // load default configuration files, if present
    ServerApplication::initialize(self);
    Poco::AutoPtr<Poco::FileChannel> fCh(new Poco::FileChannel);
#ifndef DEBUG
    fCh->setProperty("path", "realPath.log");
    fCh->setProperty("rotation", "daily");
    fCh->setProperty("archive", "timestamp");
    fCh->setProperty("times", "local");
    fCh->setProperty("compress", "true");
    fCh->setProperty("purgeAge", "6 months");
#else
    fCh->setProperty("path", "ShellServer.debug.log");
#endif

    Poco::AutoPtr<Poco::PatternFormatter> formatter(new Poco::PatternFormatter);
    formatter->setProperty("times", "local");
    formatter->setProperty("pattern", "%Y-%m-%d %H:%M:%S.%F%z [%s]: [%p] -> %t");
    Poco::AutoPtr<Poco::FormattingChannel> fChannel(new Poco::FormattingChannel(formatter, fCh));
    Poco::AutoPtr<Poco::AsyncChannel> aCh(new Poco::AsyncChannel(fChannel));
    self.logger().setChannel(aCh);
    std::cout << self.logger().name() << std::endl;
#ifdef DEBUG
    self.logger().setLevel(Poco::Message::PRIO_DEBUG);
#else
    self.logger().setLevel(Poco::Message::PRIO_INFORMATION);
#endif
}

void WebSocketServer::uninitialize()
{
    ServerApplication::uninitialize();
}

void WebSocketServer::defineOptions(OptionSet &options)
{
    ServerApplication::defineOptions(options);

    options.addOption(
        Option("help", "h", "display help information on command line arguments")
            .required(false)
            .repeatable(false));
}

void WebSocketServer::handleOption(const std::string &name, const std::string &value)
{
    ServerApplication::handleOption(name, value);

    if(name == "help")
        _helpRequested = true;
}

void WebSocketServer::displayHelp()
{
    HelpFormatter helpFormatter(options());
    helpFormatter.setCommand(commandName());
    helpFormatter.setUsage("OPTIONS");
    helpFormatter.setHeader("This is the Poco ServerShell. A dummy server for developing applications.");
    helpFormatter.format(std::cout);
}

int WebSocketServer::main(const std::vector<std::string>& args)
{
    if (_helpRequested){
        displayHelp();
    }
    else{
        // get parameters from configuration file
        unsigned short port = (unsigned short) config().getInt("WebSocketServer.port", 43880);
        ThreadPool pool(128, 256, 60, POCO_THREAD_STACK_SIZE);
#ifdef SECSRV
        Poco::Net::initializeSSL();
        Poco::SharedPtr<Poco::Net::InvalidCertificateHandler> pCert =
            new Poco::Net::ConsoleCertificateHandler(false);
        Poco::Net::Context::Ptr pContext = new Poco::Net::Context(
                Poco::Net::Context::SERVER_USE,
                "key.pem",
                "cert.pem",
                "",
                Poco::Net::Context::VERIFY_RELAXED,
                9,
                false,
                "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
        Poco::Net::Context *ptr = pContext;
        ptr->requireMinimumProtocol(Poco::Net::Context::PROTO_TLSV1_2);
        ptr = nullptr;
        Poco::Net::SSLManager::instance().initializeClient(0, pCert, pContext);
        // set-up a server socket
        SecureServerSocket svs(port, 128, pContext);
#else
        ServerSocket svs(port);
#endif
        // set-up a HTTPServer instance
        HTTPServer srv(new RequestHandlerFactory, pool, svs, new HTTPServerParams);
        // start the HTTPServer
        srv.start();
        // wait for CTRL-C or kill
        waitForTerminationRequest();
        // Stop the HTTPServer
        srv.stop();
    }
    return Application::EXIT_OK;
}
