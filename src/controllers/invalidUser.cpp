#include "export.h"

HANDLER invalidUser = [](Request *req, Response *res, bool *next) {
    res->asDefault404();
    JSON json ;
    json["error"] = "Invalid user token";
};