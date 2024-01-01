
#include "../lib/sqlite3.h"
#include<iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <vector>
#include <ctime>
#include <chrono>

#include "../lib/json.h"
#pragma once

#define DEVELOPMENT "./db/development.db"
#define PRODUCT "esp32.db"
#define SUCCESS 1
#define FAIL 0

#define ROWMAX 1000

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
    class Record recordWithID(int ID);
    std::vector<Record> recordsWithTimestamp(long int begin,long int end);
    std::vector<Record> recordsWithBeginTime(long int begin, int range);
    std::vector<Record> recordsWithEndTime(long int end, int range);
    
    /* Add data to database - return SQLITE_DONE if saving sucessfully */
    int addData(int deviceID, std::string vol, int time);
    int latestTimestamp();
    int oldestTimestamp();
    int deleteRecords(int espID, int begin, int end);
    int allToCSV() ;

    
    
    
    // int getData
};

/* TO-DO: Implement Request Row class */ 
class Record {
    private:
    int _id,_espID, _timestamp;
    std::string _rawVol;

    public:
    Record (int id, int espID, char* rawVol, int timestamp);
    int getID();
    int getEspID();
    std::vector<int> getVols();
    int getTimestamp();
    
    Json::Value toJson();

    std::string toJsonString();
    static std::string csvTitleRow();
    std::string toCsvRow();
};