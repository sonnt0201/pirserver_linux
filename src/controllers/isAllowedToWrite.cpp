#include "export.h"

HANDLER isAllowedToWrite = [](Request *req, Response *res, bool *next) {

    JSON root;
    root["state"] = pirOrm.allowDataWrite();

    res->asJson(root);
    

};