#include "export.h"

HANDLER api_example = [](Request *req, Response *res, bool *next) {
    Json::Value root;
    root["payload"] = "hello world";

    res->asJson(root);

};