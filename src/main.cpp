#include <iostream>
#include "../modules/modules.h"

#define PORT 8080

int main()
{
    Server app = Server(PORT);

    app.get("*", [](Request *req, Response *res, bool *next)
            { 
                res->setContentType(TEXT_HTML);
                res->asHtmlFile("../web-views/index.html");
                std::string agent = req->headerValue("Content-Type");
                std::cout<<agent<<std::endl;
            });

    app.get("another/*", [](Request *req, Response *res, bool *next) {
        res->setContentType(TEXT_HTML);
        res->asHtmlFile("../web-views/index.html");

    });

    app.post("/api/v1", [](Request *req, Response *res, bool *next) {
        Json::Value json = req->toJson();
        std::string type = req->headerValue("Content-Type");
        std::cout<<json<<std::endl;

    });
    app.run();
}