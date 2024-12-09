#include "export.h"

/*
{
    group: ID,
    begin: number,
    end: number
}
*/
HANDLER getRecordsOfGroup = [](Request *req, Response *res, bool *next)
{
    

    ID group = req->queryValue("group");

    String beginStr = req->queryValue("begin"); 
    uint64_t begin;
    if (beginStr != "") begin = stoull(req->queryValue("begin")) ; else begin = 0;

    String endStr = req->queryValue("end");

    uint64_t end = (endStr != "" ) ? stoull(endStr) : (time(NULL) * 1000);
    // if (end == NULL) end = time(NULL);

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