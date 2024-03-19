#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>

#include "define.h"




#pragma once


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

        // Set body as html content. Automatically set Content-Type as "text/html".
        void asHtml(std::string text);

        // Set body as html content from a specified html file. Automatically set Content-Type as "text/html".
        void asHtmlFile(std::string fileName);
        
        // set header for response
        void setHeader(std::string key, std::string value);

        // Set body as json content Automatically set Content-Type as "application/json" 
        void asJson(Json::Value root);
        
        // Set body as plain text. Default Content-Type as "text/plain".
        void asPlainText(std::string text);

        // Set body as csv content. Automatically set Content-Type as "
        void asCsv(std::vector<std::string> rows);
        // get full raw text of the response
        char* rawText() ;
        
        // append content to response body. Can be called multiple times
        void write(std::string content);
        
        // clear body
        void clearBody();
        
        // 404 Page
        void asDefault404();

        // bad request
        void asDefaultBadRequest();

        // redirect
        void redirect(std::string link);

};