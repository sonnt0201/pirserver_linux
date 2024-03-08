#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>

#include "define.h"




#pragma once

const std::string ROOT = "../";

class Response {
    private:
        // Set statusCode string when init
        
        
        std::string _statusCode;
        std::string _body = "No body sent in response !";
        std::string accessControl = "";
        std::map<std::string, std::string> _header;
    public:

        // Initialization
        Response(int status, std::string contentType );

        void setStatusCode(int status);

        void setContentType(std::string type);

       

        void asHtmlFile(std::string fileName);
        // body content for response
        
        void setHeader(std::string key, std::string value);

        void setJsonBody(Json::Value root);
        
        void setPlainBody(std::string text);

        void setCsvBody(std::vector<std::string> rows);
        // get full raw text of the response
        char* rawText() ;

        
        

       
        
        
};