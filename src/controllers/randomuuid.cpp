
#include "export.h"

HANDLER ramdomuuid = [](Request *req, Response *res, bool *next)
{
    Json::Value root;
    root["id"] = uuid_v4();
    res->asJson(root);
};