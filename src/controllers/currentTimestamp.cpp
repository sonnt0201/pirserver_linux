#include "export.h"

HANDLER currentTimestamp = [](Request* req, Response* res, bool* next) {
    JSON resJson ;
    
    Timer timer;

    resJson["timestamp"] = timer.nowInMillisec();
   

    res->asJson(resJson);
};