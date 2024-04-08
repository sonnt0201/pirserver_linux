#include <netinet/in.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>
#include "Timer.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Router.hpp"



#pragma once



class Server {
    private:
    int port;
    int serverSocket;
    void onClientConnection(int clientSocket);
    Router _router = Router();
   
    public:
    Server(int port);
    void run();
    void use(Router router);
    void get( std::string endpoint, HANDLER handler);
    void post(std::string endpoint, HANDLER handler);
    void put(std::string endpoint, HANDLER handler);
    void del(std::string endpoint, HANDLER handler);

};