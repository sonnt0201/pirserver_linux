
#include "export.h"

HANDLER home = [](Request* req, Response *res, bool *next) {
    res->asHtmlFile("../web-views/index.html");
};