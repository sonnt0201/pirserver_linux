#include "export.h"

HANDLER createPir = [](Request *req, Response *res, bool *next) {
    JSON reqJson = req->toJson();

    if (
        pirOrm.isGroupExists(reqJson["group"].asString())
    ) {
          JSON resJson;
    ID newpir;
    int rc = pirOrm.createPIR(reqJson["group"].asString(), reqJson["description"].asString(),  &newpir);

    if (rc != SUCCESS) {
        res->asDefault404();
        return;

    }

    resJson["new_pir"] = newpir;

    res->asJson(resJson);
       
    } else {
         res->asDefault404();
        return;
    }

  

    
};