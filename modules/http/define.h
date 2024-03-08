
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
#include "../lib/json.h"
#define TEXT_HTML "text/html"
#define APPLICATION_JSON "application/json"
#define TEXT_PLAIN "text/plain"
#define APPLICATION_URLENCODED "application/x-www-form-urlencoded"
#define TEXT_CSV "text/csv"
#define GET 0
#define POST 1
#define PUT 2
#define DEL 3
#define INVALID_METHOD -1

typedef int SOCKET;

#pragma once

