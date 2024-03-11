#include "Server.hpp"

bool endpointMatched(Request& req,MiddleWare& mw);

Server::Server(int port)
{
    // bind port
    this->port = port;

    ssize_t valread;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);

    // Create socket file descriptor
    if ((this->serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Failed to init socket server");
        exit(EXIT_FAILURE);
    }

    // try to bind to port 8080
    if (setsockopt(this->serverSocket, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
}

void Server::use(Router router)
{
    this->router = router;
};

void Server::run()
{

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(this->port);

    // Forcefully attaching socket to the port 8080
    if (bind(this->serverSocket, (struct sockaddr *)&address,
             sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(this->serverSocket, SOMAXCONN) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is listening on port: " << this->port << std::endl;

    int valRead;
    char request[8000] = "";
    int clientSocket;
    while (1)
    {
        if ((clientSocket = accept(this->serverSocket, NULL,
                                   NULL)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        std::thread handleConnection([this](int clientSocket)
                                     { this->onClientConnection(clientSocket); },
                                     clientSocket);

        handleConnection.detach();
    }
    close(serverSocket);
};

// Callback function to run async
void Server::onClientConnection(int clientSocket)
{
    int valRead;
    char request[8000] = "";
    while (
        (valRead = read(clientSocket, request, sizeof(request))) > 0)
    {
        Timer timer = Timer();
        timer.start();
        Request req = Request(request);
        Response res = Response(200, TEXT_PLAIN);
        bool hasMiddleware = false;
        bool next = false;

        for (MiddleWare mw : this->router.getMiddleWares())
        {
            // Check all invalid cases, if invalid, continue
            if (!endpointMatched(req, mw)) continue;

            // TO-DO: Set a timer here to count handling time. 
            

            hasMiddleware = true;
                mw.handler(&req, &res, &next);

          

            if (!next)
                break;
        }

        if (!hasMiddleware)
        {
            res.asDefault404();
        }

        // send to client
        char *text = res.rawText();
        send(clientSocket, text, strlen(text), 0);


        // stop timer
        timer.stop();
        std::cout<<"______________________________\n"
        <<timer.now()
        << req.methodAsString()<<" "<<req.path()
        << "\nFinished handling request in: "<<timer.getDuration()<<" milliseconds\n";

        close(clientSocket);
    }
}

void Server::get( std::string endpoint, HANDLER handler) {
    this->router.get(endpoint, handler);
};
void Server::post(std::string endpoint, HANDLER handler) {
    this->router.post(endpoint, handler);
};
void Server::put(std::string endpoint, HANDLER handler) {
   this->router.put(endpoint, handler);
};
void Server::del(std::string endpoint, HANDLER handler) {
  this->router.del(endpoint, handler);
};

bool endpointMatched(Request &req, MiddleWare &mw) {
    
    if (req.method() != mw.method) return false;

    // Check for * first
    if (mw.endpoint == "*") return true;

    if (mw.endpoint[mw.endpoint.length() - 1] == '*') {
        std::string destinationPath = mw.endpoint.substr(0, mw.endpoint.length() - 1);
        if (destinationPath == req.path().substr(0, destinationPath.length())) return true;

    }

    if (mw.endpoint == req.path()) return true;

    return false;
}