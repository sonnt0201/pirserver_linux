#include "Response.hpp"

Response::Response(int status, std::string contentType)
{
    
    this->setStatusCode(status);

    // set content type
    this->_header["Content-Type"] = contentType;
    this->_header["Access-Control-Allow-Origin"] = "*";
    
}

void Response::setStatusCode(int status){
    switch (status)
    {
    case 200:
        this->_statusCode = "200 OK";
        break;
    case 404:
        this->_statusCode = "404 Not Found";
        break;
    case 400: 
        this->_statusCode = "400 Bad Request";
        break;
    case 301:
        this->_statusCode = "301 Moved Permanently";
        break;
    case 302:
        this->_statusCode = "302 Found";
        break;
    case 303:
        this->_statusCode = "303 See Other";
        break;
    default:
        this->_statusCode = "500 Internal Server Error";
        break;
    }
}

void Response::setHeader(std::string key, std::string value) {
    this->_header[key] = value;
    
}

char *Response::rawText()
{
    std::string header = "";
    header = "HTTP/1.1 " 
    + _statusCode + "\n"; 
    
    for (auto const& element: this->_header) {
        header += element.first + ": " + element.second + "\n";
    }

    // endline twice for header's end
    header += "\n";

    std::string text = header + this->_body;

    // Allocate memory for the C-style string (char array) and copy the content
    char *chartext = strdup(text.c_str());

    return chartext;
}

// void Response::sendClient(int client)
// {
//     char *text = this->rawText();
//     send(client, text, strlen(text), 0);
// }

void Response::setContentType(std::string type){
    this->_header["Content-Type"] = type;
};

void Response::asHtml(std::string text) {
    // set content type
    this->_header["Content-Type"] = TEXT_HTML;
    this->_body = text;
};

void Response::asHtmlFile(std::string fileName)
{
    // Guard
    this->_header["Content-Type"] = TEXT_HTML;
    // Assuming htmlPath is the root directory followed by the fileName
    std::string fullPath = fileName;

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
              
        std::cout<<"Error: Unable to open file "<<fullPath<<std::endl;
        this->_header["Content-Type"] = TEXT_HTML;
        this->_statusCode = "404 Not Found";
        this->_body = "Not Found";
        // If the file couldn't be opened, set an appropriate message in the body
     
        
    }
}


void Response::asJson(Json::Value root) {
    // Set the Content-Type
    this->_header["Content-Type"] = APPLICATION_JSON;
    // Cast Json::Value to std::string
    Json::FastWriter fastWriter;
    std::string output = fastWriter.write(root);

    this->_body = output;
}

void Response::asPlainText(std::string text){
    this->_body = text;
}

void Response::asCsv(std::vector<std::string> rows)  {
    // set the Content-Type
    this->_header["Content-Type"]!= TEXT_CSV;
    this->_body = "";
    this->_header["Content-Disposition"] = "attachment; filename=pirdata.csv";

    for (auto& row: rows) {
        this->_body += row + "\n";
    }
    return;
    
 }

void Response::write(std::string content) {
    this->_body = _body + content;
}

void Response::clearBody() {
    this->_body.clear();
}

void Response::asDefault404() {
    // Guard
    this->_header["Content-Type"] = TEXT_HTML;
    // Assuming htmlPath is the root directory followed by the fileName
    std::string fullPath = DEFAULT_404;

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
        std::cout<<"Error: Unable to open file "<<fullPath<<std::endl;
        this->_header["Content-Type"] = TEXT_PLAIN;
        this->_statusCode = "404 Not Found";
        this->_body = "Not Found";
        // If the file couldn't be opened, set an appropriate message in the body
        
    }
    
 }

void Response::asDefaultBadRequest() {
    this->_statusCode = "400 Bad Request";
    this->setContentType(TEXT_PLAIN);
    this->asPlainText("Opps! Bad Request.");
}

void Response::redirect(std::string link) {
    this->setStatusCode(302);
    this->_header[LOCATION] = link;

}