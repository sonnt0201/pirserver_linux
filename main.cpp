#include "./controllers/Server.hpp"

#define PORT 8080

int main(){
    Server server = Server(8080);
    server.run();
}