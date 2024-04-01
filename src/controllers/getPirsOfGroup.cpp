#include "export.h"

/*
    "group_id": String
*/
HANDLER getPirsOfGroup = [](Request *req, Response *res, bool *next) {
    JSON reqJson;
    reqJson = req->toJson();

    ID groupId = reqJson["group_id"].asString();

    std::vector<PIR> pirs;

    pirs = pirOrm.readPIRs(groupId);

    JSON resJson ;
    resJson["payload"]= Json::arrayValue;
    for (PIR &pir: pirs){
        resJson["payload"].append(pir.toJson());

    }

    res->asJson(resJson);


};