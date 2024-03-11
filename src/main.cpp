#include <iostream>
#include "../modules/modules.h"

#define PORT 8081

int main()
 {
    Server app = Server(PORT);

    app.get("api/example", [](Request *req, Response *res, bool *next)
            { 
               
                Json::Value root;
                root["data"] = "hello world";

                res->asJson(root);
            });

   

    app.run();
}