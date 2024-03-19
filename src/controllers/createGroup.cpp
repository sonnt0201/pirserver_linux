
#include "export.h"

HANDLER createGroup = [](Request *req, Response *res, bool *next) {
    JSON reqJson;
    reqJson = req->toJson();
    
    String description = reqJson["description"].asString();
    JSON resJson;
    ID newid;
    pirOrm.createGroup(description, &newid);
    resJson["new_group"] = newid;

    res->asJson(resJson);

    
};