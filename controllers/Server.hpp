#include <netinet/in.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "Controller.hpp"
#include "Filter.hpp"
#include "../models/Request.hpp"
#include "../models/Response.hpp"
#include "../models/PIRDB.hpp"



#pragma once

class Server {
    private:
    int port;
    int serverSocket;
    public:
    Server(int port);
    void run();

};