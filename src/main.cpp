#include <iostream>
#include "../modules/modules.h"

#define PORT 8080

int main()
{
    Server app = Server(PORT);

    app.get("/hello", [](Request *req, Response *res, bool *next)
            { 
                res->setContentType(TEXT_HTML);
                res->setHtmlBody("../web-views/index.html");
                std::string agent = req->headerValue("Content-Type");
                std::cout<<agent<<std::endl;
            });

    app.run();
}