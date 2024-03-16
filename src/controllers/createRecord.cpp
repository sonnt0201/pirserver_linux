#include "export.h"

HANDLER createRecord = [](Request *req, Response *res, bool *next) {
    JSON reqJson = req->toJson();
    std::cout<<reqJson<<std::endl;
    JSON resJson;
    int rc;
    try {
         rc = pirOrm.createRecord(reqJson["pir"].asString(), reqJson["vol"].asString(), reqJson["timestamp"].asInt());
    
    } catch (const std::exception& e) {
         std::cerr << "Error: Unexpected exception: " << e.what() << std::endl;
            return;
    };

   if (rc == SUCCESS) {
        resJson["status"] = "OK";
        res->asJson(resJson);
        return;
    } 

    res->setStatusCode(400);
    resJson["status"] = "FAILED";
    res->asJson(resJson);
    return;

};