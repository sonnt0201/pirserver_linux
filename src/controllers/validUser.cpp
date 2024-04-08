
#include "export.h"

HANDLER validUser = [](Request *req, Response *res, bool *next) {
    JSON json = req->toJson();
    
    JSON resJson ;

    String userToken = json["user_token"].asString();

    resJson["valid"] = pirOrm.validUser(userToken);

    res->asJson(resJson);
    std::cout<<"User: "<<userToken<<std::endl;
    

};

