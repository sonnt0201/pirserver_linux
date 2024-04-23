#include <iostream>
#include "../modules/export.h"
#include "config.h"
#include "controllers/export.h"
#include "utils/export.h"
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
    createRecordsOfGroup,
    getPir,
    getUserInfo,
    getPirsOfGroup,
    getGroupList,
    legacyCreateRecord,
    getLatestRecords
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
    route.get("api/pir", getPir);
    route.get("api/user-info", getUserInfo);
    route.get("api/pirs/group", getPirsOfGroup);
    route.get("api/all-groups", getGroupList);
    route.get("api/records/latest", getLatestRecords);
    //    route.get("/invalid-user", )

    // POST
    // authen goes firsts
    
    // route.post("*", userAuthen);
    route.post("/api/user", validUser);
    route.post("api/new-group", createGroup);
    route.post("api/new-pir", createPir);
    // one pir
    route.post("api/new-record", createRecord);
    route.post("api/new-records", createRecordsOfGroup);
    
    std::cout << "Router initialized." << std::endl;

    app.use(route);

    printNezuko();

    app.run();
}