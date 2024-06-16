#include "export.h"


HANDLER labelData = [](Request *req, Response *res, bool *next) {
    std::cout<<req->getText()<<std::endl;
};