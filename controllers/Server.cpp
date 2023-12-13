#include "Server.hpp"

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

    ssize_t valRead;
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

        while (
            (valRead = read(clientSocket, request, sizeof(request))) > 0)
        {
            // Start timer
            time_t now = time(0);
            std::cout << "_______________ \n"
                      << ctime(&now);
            auto start = std::chrono::high_resolution_clock::now();

            // Init request object
            Request req = Request(request);
            int method = req.method();
            std::string strMethod = (method == GET) ? "GET" : (method == POST) ? "POST"
                                                          : (method == PUT)    ? "PUT"
                                                                               : "DELETE";

            // Delegate to filter and controller
            bool valid = filter(req);
            if (valid)
                controller(clientSocket, req);
            close(clientSocket);

            // Counting time
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            std::cout << "Finish handling " << strMethod << " request in: " << duration.count() << " millisecs" << std::endl
                      << std::endl;
        }
    }
    close(serverSocket);
    
};