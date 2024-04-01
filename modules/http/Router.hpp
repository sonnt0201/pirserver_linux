
#include "define.h"
#include "Request.hpp"
#include "Response.hpp"

#pragma once
/*
    Handler function for HTTP Middleware.
    next = false as default.
*/
typedef std::function<void(Request* req, Response* res ,bool* next)> HANDLER;

/*
    Middleware is an object that holds a function to handle a request with specific method and endpoint, also create response to be sent.
    HANDLER is a callback function taking over that handling.
*/
struct MiddleWare {
    int method = -1;
    std::string endpoint = "";
    HANDLER handler;
};

/*
    Router is an object that holds many middlewares for server to handle.
    Router also provide functions (get, post, put, delete) to add middlewares (also called mappers).
*/
class Router {
    private:
    std::vector<MiddleWare> middlewares;

    public:
    Router();
    // add get middleware
    void get( std::string endpoint, HANDLER handler);
    // add post middleware
    void post(std::string endpoint, HANDLER handler);
    // add put middleware
    void put(std::string endpoint, HANDLER handler);
    // add delete middleware
    void del(std::string endpoint, HANDLER handler);
    // get all middleware
    std::vector<MiddleWare> getMiddleWares();
};
