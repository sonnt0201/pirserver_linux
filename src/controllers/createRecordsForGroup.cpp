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
HANDLER createRecordsOfGroup = [](Request *req, Response *res, bool *next)
{
    // req->closeConnection();

    JSON reqJson = req->toJson();
    if (reqJson == NULL) {
          res->asDefault404();
            // res->clearBody();
            // res->write("400 Bad request");

    }
    JSON records = reqJson["records"];

    res->setStatusCode(200);
    res->setContentType(TEXT_PLAIN);
    res->asPlainText("Received !");
    int timestamp = reqJson["timestamp"].asInt();
    if (records.size() == 5)
        for (JSON &recordJson : records)
        {
            ID pir = recordJson["pir"].asString();
            ID vol = recordJson["vol"].asString();

            int rc = pirOrm.createRecord(pir, vol, timestamp);

            if (rc != SUCCESS)
            {
                res->setStatusCode(400);
                res->setContentType("text/plain");
                res->asPlainText("Oops! Bad request!");
            }
        } else {
            res->asDefault404();
            // res->clearBody();
            // res->write("400 Bad request");

            // req->closeConnection();
        }
};