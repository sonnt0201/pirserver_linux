#include "export.h"
/*
{
    "group": ID,
    "number": number 
}
*/
HANDLER getLatestRecords = [](Request *req, Response * res, bool *next) {
    ID group = req->queryValue("group");
    int number = stoi(req->queryValue("number"));
    
    std::vector <class Record> records = pirOrm.latestRecords(number, group);

    if (!pirOrm.isGroupExists(group)) {
        res->asDefault404();
        return;
    }

    JSON resJson;
    resJson["total"] = records.size();
    resJson["payload"] = Json::arrayValue;

    for (auto &record : records) {
        resJson["payload"].append(record.toJson());

    }

    res->asJson(resJson);

};