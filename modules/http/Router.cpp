#include "define.h"
#include "Router.hpp"

Router::Router() {
    // empty
}

void Router::get( std::string endpoint, HANDLER handler) {
    MiddleWare mw;
    mw.method = GET;
    mw.endpoint = endpoint;
    mw.handler = handler;
    this->middlewares.push_back( mw );
};
void Router::post(std::string endpoint, HANDLER handler) {
     MiddleWare mw;
    mw.method = POST;
    mw.endpoint = endpoint;
    mw.handler = handler;
    this->middlewares.push_back( mw );
};
void Router::put(std::string endpoint, HANDLER handler) {
     MiddleWare mw;
    mw.method = PUT;
    mw.endpoint = endpoint;
    mw.handler = handler;
    this->middlewares.push_back( mw );
};
void Router::del(std::string endpoint, HANDLER handler) {
     MiddleWare mw;
    mw.method = DEL;
    mw.endpoint = endpoint;
    mw.handler = handler;
    this->middlewares.push_back( mw );
};
std::vector<MiddleWare> Router::getMiddleWares(){
    return this->middlewares;
};