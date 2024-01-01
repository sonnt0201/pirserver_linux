#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fstream>
#include "../lib/json.h"
#include "http.h"
typedef int SOCKET ;



#pragma once

const std::string ROOT = "../";

class Response {
    private:
        // Set statusCode string when init
        void setStatusCode();
        
        std::string _statusCode;
        std::string _contentType;
        std::string _body = "No body sent in response !";
        std::string accessControl = "";
        std::map<std::string, std::string> _headers;
    public:

        // Initialization
        Response(int status, std::string contentType );
        std::string htmlPath;
        std::string jsonData;
        void setHtmlContent(std::string fileName);
        // body content for response
        
        void setHeaderParam(std::string key, std::string value);
         void setJsonContent(Json::Value root);
        void setPlainContent(std::string text);

        void setCsvContent(std::vector<std::string> rows);
        // get full raw text of the response
        char* rawText() ;

        // send response to client socket
        void sendClient(int client);
        

       
        
        
};