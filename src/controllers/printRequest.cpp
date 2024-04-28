#include "export.h"

HANDLER printRequest = [](Request *req, Response *res, bool *next) {
    std::cout <<"\n"<< req->getText() << std::endl<<"\n";
    *next = true;
};