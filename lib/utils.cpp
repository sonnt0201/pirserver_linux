#include "utils.hpp"

void send404(SOCKET client) {
    Response response = Response(404, TEXT_HTML);
    response.setHtmlContent("404.html");
    response.sendClient(client);
}

// Bad Request, only use this function when the filter return 0
void send400(SOCKET client) {
    std::cout<<"\nFilter: Bad request! \n";
    Response response = Response(400, TEXT_PLAIN);
    response.setPlainContent("OOPS!.. Bad Request! ");
    response.sendClient(client);
}
// Cast string to unsigned long int, if string is empty, return 0, if invalid string, return -1
long int stringToUInt(std::string string) {
    if (string == "") return 0;
    if (string.length() > 10)  return -1;
    long int n = 0;
    for (int i = 0; i < string.length() ; i++) {
        if (string[i] < '0' || string[i] > '9') return -1;
        n = n*10 + string[i] - '0';
    };

    return n;
}