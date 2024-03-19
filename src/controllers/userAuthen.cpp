#include "export.h"

HANDLER userAuthen = [](Request *req, Response *res, bool *next) {
    JSON reqJson = req->toJson();
    ID userToken = reqJson["user_token"].asString();
    
    if (pirOrm.validUser(userToken)) {
        *next = true;
      
    } else {
        JSON resJson;
        resJson["Error"] = "Authentication failed";
        res->asJson(resJson);
        *next = false;
    }
};