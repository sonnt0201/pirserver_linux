#include "export.h"

/*
    ?group_id:String
*/
HANDLER getPirsOfGroup = [](Request *req, Response *res, bool *next) {
    ID groupId = req->queryValue("group_id");

    std::vector<PIR> pirs;

    pirs = pirOrm.readPIRs(groupId);

    JSON resJson ;
    resJson["payload"]= Json::arrayValue;
    for (PIR &pir: pirs){
        resJson["payload"].append(pir.toJson());

    }

    res->asJson(resJson);


};