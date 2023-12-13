#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fstream>

typedef int SOCKET ;

#pragma once

const std::string ROOT = "../";

class Response {
    private:
        // Set statusCode string when init
        void setStatusCode();
        
        std::string statusCode;
        std::string contentType;
        std::string accessControl = "";
        std::string setJsonContent();
    public:

        // Initialization
        Response(int status, std::string contentType );
        std::string htmlPath;
        std::string jsonData;
        void setHtmlContent(std::string fileName);
        // body content for response
        std::string body = "No body sent in response !";
        
        // get full raw text of the response
        char* rawText() ;

        // send response to client socket
        void sendClient(int client);
};