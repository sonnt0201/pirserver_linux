#include "export.h"

HANDLER getGroupList = [](Request* req, Response* res, bool* next) {
    JSON resJson = Json::arrayValue;
    
    std::vector<class PirGroup> groups = pirOrm.readAllGroups();

    for (PirGroup group : groups) {
        resJson.append(group.toJson());
    }

    res->asJson(resJson);
};