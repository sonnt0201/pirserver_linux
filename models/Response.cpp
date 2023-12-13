#include "Response.hpp"

Response::Response(int status, std::string contentType)
{
    switch (status)
    {
    case 200:
        this->statusCode = "200 OK";
        break;
    case 404:
        this->statusCode = "404 Not Found";
        break;
    default:
        this->statusCode = "500 Internal Server Error";
        break;
    }

    this->contentType = contentType;
}

char *Response::rawText()
{
    std::string header = "";
    header = "HTTP/1.1 " + statusCode + "\n" + "Content-Type: " + this->contentType + "\n" + "Access-Control-Allow-Origin: *" + "\n\n";

    std::string text = header + this->body;

    // Allocate memory for the C-style string (char array) and copy the content
    char *chartext = strdup(text.c_str());

    return chartext;
}

void Response::sendClient(int client)
{
    char *text = this->rawText();
    send(client, text, strlen(text), 0);
}

void Response::setHtmlContent(std::string fileName)
{
    // Assuming htmlPath is the root directory followed by the fileName
    std::string fullPath = "../web-views/" + fileName;

    // Open the file
    std::ifstream file(fullPath);

    if (file.is_open())
    {
        // Read the content into a string
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        // Assign the content to the body member variable
        this->body = content;
        // std::cout<<content<<std::endl;
        // Close the file
        file.close();
    }
    else
    {
        // If the file couldn't be opened, set an appropriate message in the body
        this->body = "Error: Unable to open file " + fileName;
    }
}