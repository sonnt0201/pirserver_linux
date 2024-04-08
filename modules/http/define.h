
#include <iostream>
#include <stdio.h>
#include <vector>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fstream>
#include <algorithm>
#include <functional>
#include "json.h"

// HTTP Content types
#define TEXT_HTML "text/html"
#define APPLICATION_JSON "application/json"
#define TEXT_PLAIN "text/plain"
#define APPLICATION_URLENCODED "application/x-www-form-urlencoded"
#define TEXT_CSV "text/csv"

// HTTP Request Methods

#define GET 0
#define POST 1
#define PUT 2
#define DEL 3
#define INVALID_METHOD -1

// Header Keys
#define HOST "Host"
#define LOCATION "Location"
#define CONTENT_TYPE "Content-Type"

// type def 
typedef int SOCKET;
typedef std::string String;
typedef Json::Value JSON;

// other resources
#include "config.h"
#pragma once

