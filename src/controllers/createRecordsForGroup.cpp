#include "export.h"

/*
{
    "timestamp": Number,

    "records": [
        {
            "pir": String,
            "vol": String,
            
        },
        ...
    ]
}
*/
HANDLER createRecordsOfGroup = [](Request *req, Response *res, bool* next) {
    JSON reqJson = req->toJson();

    JSON records = reqJson["records"];

    res->setStatusCode(200);
    res->setContentType(TEXT_PLAIN);
    res->asPlainText("Received !");
    int timestamp = reqJson["timestamp"].asInt();
    for (JSON &recordJson: records) {
        ID pir = recordJson["pir"].asString();
        ID vol = recordJson["vol"].asString();
        

        int rc = pirOrm.createRecord(pir, vol, timestamp);

        if (rc != SUCCESS) {
            res->setStatusCode(400);
            res->setContentType("text/plain");
            res->asPlainText("Oops! Bad request!");

        }


    }
}; 