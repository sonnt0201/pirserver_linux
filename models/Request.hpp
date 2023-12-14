
#include <iostream>
#include <string>
#include <vector>
#include <string.h>
#include <stdio.h>
#include "http.h"
#pragma once


// HTTP Request class - contains read-only attributes and methods
class Request {
    private:
        // raw text of the request in std::string type
        std::string rawText;
       
    public:

    // Initializer with char* request received from client 
    Request(char request[8000]); 

    // METHOD of HTTP Request (GET - POST - PUT - DELETE)
    int method();

    //Path of the Request
    std::string path();

    // List of the parameters in the request
    std::vector<std::string> params () ;

    // Get the value of a parameter in the request
    std::string value(std::string param);

    //Body content of the request
    std::string body();

    // return origin request in raw text
    std::string getText() ;
};
