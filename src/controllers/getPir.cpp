#include "export.h"

/*
    Query: 
    pir_id=String
*/
HANDLER getPir = [](Request *req, Response *res, bool* next) {
    ID pirId = req->queryValue("pir_id");
    std::cout<<pirId<<std::endl;
    PIR pir = PIR("", "", "");
    int rc = pirOrm.readPIR(pirId, &pir);

    if (rc == FAIL) {

        res->asDefaultBadRequest();
        JSON json;
        json["error"] = "Invalid PIR id";
        res->asJson(json);
        return;
    }

    res->asJson(pir.toJson());
    return;

};