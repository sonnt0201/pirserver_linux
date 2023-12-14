#include "Response.hpp"

Response::Response(int status, std::string contentType)
{
    switch (status)
    {
    case 200:
        this->_statusCode = "200 OK";
        break;
    case 404:
        this->_statusCode = "404 Not Found";
        break;
    default:
        this->_statusCode = "500 Internal Server Error";
        break;
    }

    // set content type
    this->_headers["Content-Type"] = contentType;
    this->_headers["Access-Control-Allow-Origin"] = "*";
    
}

void Response::setHeaderParam(std::string key, std::string value) {
    this->_headers[key] = value;
    
}

char *Response::rawText()
{
    std::string header = "";
    header = "HTTP/1.1 " 
    + _statusCode + "\n"; 
    
    for (auto const& element: this->_headers) {
        header += element.first + ": " + element.second + "\n";
    }

    // endline twice for header's end
    header += "\n";

    std::string text = header + this->_body;

    // Allocate memory for the C-style string (char array) and copy the content
    char *chartext = strdup(text.c_str());

    return chartext;
}

void Response::sendClient(int client)
{
    char *text = this->rawText();
    send(client, text, strlen(text), 0);
}

// fileName param must be html file put in 'web-views' folder, NOT including any root dir prefix
void Response::setHtmlContent(std::string fileName)
{
    // Guard
    if (this->_headers["Content-Type"] != TEXT_HTML) {
        std::cout<<"Wrong Content-Type to set HTML content.\n";
        return;
    }

    // Assuming htmlPath is the root directory followed by the fileName
    std::string fullPath = "../web-views/" + fileName;

    // Open the file
    std::ifstream file(fullPath);

    if (file.is_open())
    {
        // Read the content into a string
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        // Assign the content to the body member variable
        this->_body = content;
        // std::cout<<content<<std::endl;
        // Close the file
        file.close();
    }
    else
    {
        // If the file couldn't be opened, set an appropriate message in the body
        this->_body = "Error: Unable to open file " + fileName;
    }
}

void Response::setJsonContent(Json::Value root) {
    // Guard the Content-Type
    if (this->_headers["Content-Type"] != APPLICATION_JSON) {
        std::cout<<"Wrong Content-Type to set JSON content.\n";
        return;
    }
    // Cast Json::Value to std::string
    Json::FastWriter fastWriter;
    std::string output = fastWriter.write(root);

    this->_body = output;
}

void Response::setPlainContent(std::string text){
    if (this->_headers["Content-Type"] != TEXT_PLAIN) {
        std::cout<<"Wrong Content-Type to set plain-text content.\n";
        return;
    }

    this->_body = text;
}

