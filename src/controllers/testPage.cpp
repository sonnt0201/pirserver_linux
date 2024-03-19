#include "export.h"

HANDLER testPage = [](Request *req, Response *res, bool *next) {
    res->asHtmlFile("../web-views/test.html");
};