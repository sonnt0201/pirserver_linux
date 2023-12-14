#include "Request.hpp"

Request::Request(char rawText[8000])
{
    this->rawText = std::string(rawText);
}

int Request::method()
{
    std::string text = this->rawText;
    while (text[0] == ' ') text.erase(0,1);
    if (text.substr(0, 3) == "GET")
        return GET;
    if (text.substr(0, 4) == "POST")
        return POST;
    if (text.substr(0, 3) == "PUT")
        return PUT;
    if (text.substr(0, 6) == "DELETE")
        return DEL;
    
    return INVALID_METHOD;
}

std::string Request::path()
{
    std::string request = this->rawText;
    // Find the start and end index of the path in the request string
    size_t start = request.find(" ") + 1;
    size_t nextSpace = request.find(" ", start);
    size_t nextQues = request.find("?", start);

    size_t end = std::min(nextSpace, nextQues);
    
    // Extract the substring representing the path
    std::string path = request.substr(start, end - start);

    return path;
}

std::string Request::value(std::string param)
{
    std::string request = this->rawText;

    // Find the position of the parameter in the request
    size_t paramPos = request.find(param + "=");

    // If the parameter is not found, return an empty string
    if (paramPos == std::string::npos)
    {
        return "";
    }

    // Move the position to the value of the parameter
    paramPos += param.length() + 1; // Add 1 to skip the '='

    // Find the end position of the value
    size_t valueEnd = request.find("&", paramPos);
    if (valueEnd == std::string::npos)
        // If there is no '&' after the parameter, consider the value until the end of the string
        valueEnd = request.find(" ", paramPos);

    if (valueEnd == std::string::npos)
        valueEnd = request.find("\n", paramPos);

    if (valueEnd == std::string::npos)
        valueEnd == request.length();

    // Extract the substring representing the value
    std::string value = request.substr(paramPos, valueEnd - paramPos);

    return value;
}

std::vector<std::string> Request::params()
{
    std::vector<std::string> paramNames;

    // Check the request method to determine where to look for parameter names
    int methodType = this->method();

    size_t paramPos;
    size_t paramEnd;

    switch (methodType)
    {
    case 0:
    case 3:
        // For GET and DELETE, parameter names are in the query string
        paramPos = this->rawText.find("?");
        paramPos++;
        break;

    case 1:
    case 2:
        // For POST and PUT, parameter names are in the request body
        paramPos = this->rawText.find("\n\n") + 2; // Move past the empty line after headers
        break;

    default:
        // Invalid method or no parameter names found
        return paramNames;
    }

    // If there are no parameters, return an empty vector
    if (paramPos == std::string::npos)
    {
        return paramNames;
    }

    // Move the position to the start of the parameters
    // paramPos += 1;

    // Find the end position of the parameters
    size_t queryEnd = this->rawText.find(" ", paramPos);
    if (queryEnd == std::string::npos)
        queryEnd = this->rawText.find("\n", paramPos);

    // Extract the substring representing the parameters
    std::string queryParams = this->rawText.substr(paramPos, queryEnd - paramPos);

    // Find the position of '=' for each parameter and extract the name
    paramPos = 0;
    while ((paramEnd = queryParams.find("&", paramPos)) != std::string::npos)
    {
        std::string param = queryParams.substr(paramPos, queryParams.find("=", paramPos) - paramPos);
        paramNames.push_back(param);
        paramPos = paramEnd + 1; // Move to the next parameter
    }

    // Add the last parameter name
    std::string lastParam = queryParams.substr(paramPos, queryParams.find("=", paramPos) - paramPos);
    paramNames.push_back(lastParam);

    return paramNames;
}

std::string Request::body()
{
    // Find the position of the empty line after the headers
    size_t emptyLinePos = this->rawText.find("\n\n");

    // If there is no empty line, try finding an empty line with '\r\n' (Windows line ending)
    if (emptyLinePos == std::string::npos)
    {
        emptyLinePos = this->rawText.find("\r\n\r\n");
    }

    // If there is still no empty line, there might be no body, so return an empty string
    if (emptyLinePos == std::string::npos)
    {
        return "";
    }

    // Move the position to the start of the body
    emptyLinePos += 2; // Move past the empty line (2 characters for '\n\n' or '\r\n\r\n')

    // Extract the substring representing the body
    std::string requestBody = this->rawText.substr(emptyLinePos);

    return requestBody;
};

std::string Request::getText() {
        return this->rawText;
}