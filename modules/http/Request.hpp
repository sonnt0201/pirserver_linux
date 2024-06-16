
#include <iostream>
#include <string>

#include <stdio.h>
#include "define.h"
#include "config.h"
#pragma once

// HTTP Request class - contains read-only attributes and methods
class Request
{
private:
    // raw text of the request in std::string type
    std::string rawText;
    std::string _body;
    std::string _header;
    int _clientSocket = -1;
    std::map<std::string, std::string> _headerVals;
    bool _valid = true;
    // Init _header value
    void initHeader();
    // Body content of the request
    void initBody();

public:
    // Initializer with char* request received from client
    Request(char request[8000], int clientSocket);

    // check if the request is valid
    bool isValid();

    // METHOD of HTTP Request (GET | POST | PUT | DELETE)
    int method();

    std::string methodAsString();
    // Path of the Request
    std::string path();

    // Get query values from request path
    std::vector<std::string> allQueryKeys();
    // Get value of a query with key
    std::string queryValue(std::string key);
    // List of the parameters in the request
    
    std::vector<std::string> allHeaderKeys();
    std::string headerValue(std::string key);
    
    std::vector<std::string> params();

    // Get the value of a parameter in the request
    std::string value(std::string param);

    /*
        Used for application/json content-type only
        Parse request body to JSON object
    */ 
    Json::Value toJson();
    
    // return origin request in raw text
    std::string getText();

    void closeConnection();
    
    bool isSocketOpen();
};
