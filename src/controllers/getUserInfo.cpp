#include "export.h"
/*
{
    "user_token": String
}
*/
HANDLER getUserInfo = [](Request *req, Response *res, bool *next) {
    JSON reqJson = req->toJson();

    ID token = reqJson["user_token"].asString();
    User user = User("","","");
    int rc = pirOrm.readUser(token, &user);

    if (rc != SUCCESS) {
        res->asDefault404();
        JSON error;
        error["error"] = "No user found!";
        res->asJson(error);
        return;
    }

    JSON resJson;
    resJson = user.toJson();

    res->asJson(resJson);


};

