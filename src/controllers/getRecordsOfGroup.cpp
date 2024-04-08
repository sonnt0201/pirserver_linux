#include "export.h"

HANDLER getRecordsOfGroup = [](Request *req, Response *res, bool *next)
{
    JSON reqJson = req->toJson();

    ID group = reqJson["group"].asString();
    int begin = reqJson["begin"].asInt();
    if (begin == NULL) begin = 0;
    int end = reqJson["end"].asInt();
    if (end == NULL) end = time(NULL);

    std::cout <<"begin: "<<begin<<"\n";
    std::cout <<"end: "<<end<<"\n";
    if (!pirOrm.isGroupExists(group))
    {
        std::cout << "Group does not exist.\n";
        res->asDefault404();
        return;
    }

    std::vector<Record> records = pirOrm.readRecords(group, begin, end);
    std::cout<<"number: "<<records.size()<<" records\n";
    JSON resJson, payload;
    payload = Json::arrayValue;
    for (auto &record : records)
    {
        
        payload.append(record.toJson());
    }


    resJson["payload"] = payload;
    res->asJson(resJson);
};