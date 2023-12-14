#include "utils.hpp"

void send404(SOCKET client) {
    Response response = Response(404, TEXT_HTML);
    response.setHtmlContent("404.html");
    response.sendClient(client);
}