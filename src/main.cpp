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
    createRecord;

int main()
{
    Server app = Server(PORT);

    Router route = Router();

    // map all routes
    route.get("/", home);
    route.get("/api/example", api_example);
    route.get("/api/random_uuid", ramdomuuid);
    //    route.get("/invalid-user", )

    route.post("/api/user", validUser);
    route.post("api/create-group", createGroup);
    route.post("api/create-pir", createPir);
    route.post("api/create-record", createRecord);





    std::cout << "Router initialized." << std::endl;

    app.use(route);

    app.run();
}