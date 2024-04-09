
#include "export.h"

HANDLER home = [](Request* req, Response *res, bool *next) {
    JSON json;
    json["connection"] = "OK";
    res->asJson(json);
};