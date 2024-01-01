#include "../models/models.hpp"
#include "../lib/lib.hpp"
#include <string>
#include<ctime>
#include <fstream>
// if request is mapped in the current api ver, return TERMINATE to end the controller.
#define TERMINATE true
// if request is not mapped in the current api ver, return CONTINUE to continue mapping with the next mapper.
#define CONTINUE false

#define MAPPER bool

#pragma once



/*
    Controller knows nothing about server, just uses Resquest and Response
*/
void controller(int client, Request request);
