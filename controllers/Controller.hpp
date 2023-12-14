#include "../models/models.hpp"
#include "../lib/lib.hpp"
#include <string>
#include<ctime>



#pragma once



/*
    Controller knows nothing about server, just uses Resquest and Response
*/
void controller(int client, Request request);
