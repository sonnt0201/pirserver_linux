#include <iostream>
#include "../modules/export.h"
#include "config.h"
#include "controllers/export.h"

extern HANDLER validUser,
    createGroup,
    home,
    api_example,
    ramdomuuid,
    createGroup,
    createPir,
    createRecord,
    invalidUser,
    testPage,
    getRecordsOfGroup,
    userAuthen,
    createRecordsOfGroup
    ;

int main()
{
    Server app = Server(PORT);

    Router route = Router();

    // map all routes
    route.get("/", home);
    route.get("/api/example", api_example);
    route.get("/api/random_uuid", ramdomuuid);
    route.get("/invalid-user",invalidUser);
    route.get("/test", testPage);
    route.get("/api/records", getRecordsOfGroup);
    //    route.get("/invalid-user", )

    // POST
    // authen goes firsts
    route.post("*", userAuthen);
    route.post("/api/new-user", validUser);
    route.post("api/new-group", createGroup);
    route.post("api/new-pir", createPir);
    route.post("api/new-record", createRecord);
    route.post("api/new-records", createRecordsOfGroup);




    std::cout << "Router initialized." << std::endl;

    app.use(route);

    app.run();
}