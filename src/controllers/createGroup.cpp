
#include "export.h"

HANDLER createGroup = [](Request *req, Response *res, bool *next) {
    JSON reqJson;
    reqJson = req->toJson();

    if (!pirOrm.validUser(reqJson["user_token"].asString())) {
        res->asDefault404();
        return;
    }

    JSON resJson;
    ID newid;
    pirOrm.createGroup("first group", &newid);
    resJson["new_group"] = newid;

    res->asJson(resJson);

    
};