
#include "define.h"
#include "Request.hpp"
#include "Response.hpp"

#pragma once

typedef std::function<void(Request* req, Response* res ,bool* next)> HANDLER;

struct MiddleWare {
    int method;
    std::string endpoint;
    HANDLER handler;
};

class Router {
    private:
    std::vector<MiddleWare> middlewares;

    public:
    Router();
    void get( std::string endpoint, HANDLER handler);
    void post(std::string endpoint, HANDLER handler);
    void put(std::string endpoint, HANDLER handler);
    void del(std::string endpoint, HANDLER handler);
    std::vector<MiddleWare> getMiddleWares();
};
