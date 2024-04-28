#include "export.h"

HANDLER legacyCreateRecord = [](Request *req, Response *res, bool *next) {
    String id = req->value("esp-id"), vol = req->value("vol"), timeStr = req->value("time");
    // std::cout<<req->rawText()<<std::endl;
    std::cout<<req->value("vol")<<std::endl;
    int time = stoi(timeStr);
    pirOrm.createRecord(id, vol, time);
    res->asPlainText("OK");

};