
#include "../lib/sqlite3.h"
#include<iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <vector>
#include <ctime>
#include <chrono>

#pragma once

#define DEVELOPMENT "./db/development.db"
#define PRODUCT "esp32.db"
#define SUCCESS 1
#define FAIL 0

#define ROWMAX 10

// Separator for voltage data string
#define SEPARATOR '_'

// Number of voltage sent from Request
#define VOLNUM 100


/* 
    PIRDB is the database specifically configured for pir data sent from esp32 
    by default, it always has 4 column: 'id', 'esp_id', 'vol' and 'time'
*/
class PIRDB {
    private:
        sqlite3* db;
        std::string _fileName;
    public:
    /*
        REQUIRED: dbFileName must have .db extension
    */
    std::string fileName();
    PIRDB(std::string dbFileName);
    int numOfRows();
    std::vector<std::string> getDataWithID(int ID);
    int deleteAllTableContent() ;
    // TO-DO: implement method  
    class RecordRow recordWithID(int ID);

    /* Add data to database - return SQLITE_DONE if saving sucessfully */
    int addData(int deviceID, std::string vol, int time);
    
    int allToCSV() ;

    
    
    
    // int getData
};

/* TO-DO: Implement Request Row class */ 
class RecordRow {
    private:
    int _id, _espID, _timestamp, _voltage[VOLNUM];
    public:

    /* Init function */ 
    RecordRow(int id, int espID, int timestamp, int voltage[VOLNUM]);

    /* Note: id, espID, timestamp, voltage is initialized only in init function (above), after initialized, they are READONLY */     int id();
    int espID();
    int timestamp();
    std::vector <int> voltage();
};
