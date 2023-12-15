#include "PIRDB.hpp"
#include <string.h>

static int callback(void *data, int argc, char **argv, char **azColName)
{
    for (int i = 0; i < argc; i++)
    {
        std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << std::endl;
    }
    return 0;
}

PIRDB::PIRDB(std::string dbFileName)
{
    // sqlite3 *db = this->db;
    // this->db = db;
    this->_fileName = dbFileName;
    char *charf = strdup(dbFileName.c_str());
    int rc = sqlite3_open(charf, &this->db);

    const char *createTableSQL = "CREATE TABLE IF NOT EXISTS pir ("
                                 "id INTEGER AUTOINCREMENT PRIMARY KEY,"
                                 "esp_id INTEGER NOT NULL,"
                                 "vol VARCHAR NOT NULL,"
                                 "time INTEGER NOT NULL);";

    rc = sqlite3_exec(db, createTableSQL, 0, 0, 0);
}

std::string PIRDB::fileName()
{
    return this->_fileName;
}

int PIRDB::numOfRows()
{
    const char *countRowsSQL = "SELECT COUNT(*) FROM pir;";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(this->db, countRowsSQL, -1, &stmt, 0);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Cannot prepare statement: " << sqlite3_errmsg(this->db) << std::endl;
        return -1; // Return -1 to indicate an error
    }

    rc = sqlite3_step(stmt);

    if (rc == SQLITE_ROW)
    {
        int rowCount = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
        return rowCount;
    }
    else
    {
        std::cout << "Query failed: " << sqlite3_errmsg(this->db) << std::endl;
        sqlite3_finalize(stmt);
        return -1; // Return -1 to indicate an error
    }
};

std::vector<std::string> PIRDB::getDataWithID(int ID)
{
    // Start timer
    //  auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::string> result;

    const char *selectDataSQL = "SELECT esp_id, vol, time FROM pir WHERE id = ?;";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(this->db, selectDataSQL, -1, &stmt, 0);
    if (rc != SQLITE_OK)
    {
        std::cout << "Cannot prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return result; // Return an empty vector to indicate an error
    }

    // Bind the ID parameter
    sqlite3_bind_int(stmt, 1, ID);

    rc = sqlite3_step(stmt);

    if (rc == SQLITE_ROW)
    {

        result.push_back(std::to_string(sqlite3_column_int(stmt, 0))); // esp_id

        // reformat voltatge
        std::string strVol = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        for (int i = 0; i < strVol.length(); i++)
        {
            if (strVol[i] == SEPARATOR)
                strVol[i] = ',';
        }
        strVol = '[' + strVol + ']';
        result.push_back(strVol); // vol

        result.push_back(std::to_string(sqlite3_column_int(stmt, 2))); // time
    }
    else
    {
        std::cout << "Query failed with id  " << ID << ": " << sqlite3_errmsg(db) << std::endl;
    }

    // Finalize the statement
    sqlite3_finalize(stmt);

    return result;
}

int PIRDB::addData(int deviceID, std::string vol, int time)
{
    auto start = std::chrono::high_resolution_clock::now();
    const char *query = "INSERT INTO pir (esp_id, vol, time)"
                        "VALUES (?, ? , ?);";

    sqlite3_stmt *stmt;

    int rc;

    rc = sqlite3_prepare_v2(this->db, query, -1, &stmt, 0);

    if (rc != SQLITE_OK)
    {
        std::cerr << "Cannot prepare the statement: " << sqlite3_errmsg(this->db) << std::endl;
    }
    // set next id
    // int nextID = this->numOfRows() + 1;
    // sqlite3_bind_int(stmt, 1, nextID);

    sqlite3_bind_int(stmt, 1, deviceID);

    char *strVol = (char*) vol.c_str();
    sqlite3_bind_text(stmt, 2, strVol, -1, NULL);

    

    sqlite3_bind_int(stmt, 3, time);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {

        // timer
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        std::cout << "Insert data failed: " << sqlite3_errmsg(this->db) << " with duration: " << duration.count() << std::endl;
        return rc;
    }

    // timer when successful
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Add new data successfully in : " << duration.count() << " millisecs" << std::endl;

    return rc;
};

int PIRDB::deleteAllTableContent()
{
    auto start = std::chrono::high_resolution_clock::now();
    const char *query = "DELETE FROM pir;";
    sqlite3_stmt *stmt;
    int rc;
    rc = sqlite3_prepare_v2(this->db, query, -1, &stmt, 0);

    if (rc != SQLITE_OK)
    {
        std::cout << "Cannot prepare the statement: " << sqlite3_errmsg(this->db) << std::endl;
        return rc;
    }

    rc = sqlite3_step(stmt);

    // timer
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Delete table's content successfully in : " << duration.count() << " millisecs" << std::endl;

    return rc;
}

int PIRDB::allToCSV()
{
    // start timer
    auto start = std::chrono::high_resolution_clock::now();

    int count = this->numOfRows();
    if (count < ROWMAX)
    {
        // std::cout << "Row number is less than " << ROWMAX << ", can not create .csv file!\n";
        return FAIL;
    }

    time_t now = time(0);
    std::string fileName = "./csv/" + std::to_string(now);
    fileName += ".csv";

    std::ofstream csv;
    csv.open(fileName);

    for (long i = 1; i <= count; i++)
    {
        std::vector<std::string> data = this->getDataWithID(i);
        csv << data[0] << ",";
        std::string vol = data[1];
        vol.erase(0, 1);
        vol.erase(vol.length() - 1, 1);
        csv << vol;
        csv << "," << data[2] << std::endl;
    };

    // delete table's content
    // this->deleteAllTableContent();

    csv.close();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Create and write csv file in : " << duration.count() << " millisecs" << std::endl;

    return SUCCESS;
};

class Record PIRDB::recordWithID(int ID)
{

    char *query = "SELECT * FROM pir WHERE id=?";
    sqlite3_stmt *stmt;
    int rc;
    rc = sqlite3_prepare_v2(this->db, query, -1, &stmt, 0);

    if (rc != SQLITE_OK)
    {
        std::cout << "Cannot prepare statement. On recordWithID method: " << sqlite3_errmsg(this->db) << std::endl;
    }

    sqlite3_bind_int(stmt, 1, ID);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0),
            espID = sqlite3_column_int(stmt, 1),
            timestamp = sqlite3_column_int(stmt, 3);

        char *rawVols = (char *)sqlite3_column_text(stmt, 2);

        Record result = Record(id, espID, rawVols, timestamp);

        return result;
    }
    else
    {
        std::cout << "Query error: " << sqlite3_errmsg(this->db) << std::endl
                  << "With return code: " << rc << std::endl;
        return Record(-1, -1, "", -1);
    }
}

std::vector<Record> PIRDB::recordsWithTimestamp(int begin, int end){
    std::vector<Record> result = {};

    char* query = "SELECT * FROM pir WHERE time >= ? AND time <= ? ;";
    sqlite3_stmt* stmt;
    int rc;

    rc = sqlite3_prepare_v2(this->db, query, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        std::cout << "Cannot prepare statement. On recordWithTimestamp method: " << sqlite3_errmsg(this->db) << std::endl;
        return result;
    }

    sqlite3_bind_int(stmt, 1, begin);
    sqlite3_bind_int(stmt, 2, end);

    while (
        (rc = sqlite3_step(stmt)) == SQLITE_ROW
    ) {
        
         int id = sqlite3_column_int(stmt, 0),
            espID = sqlite3_column_int(stmt, 1),
            timestamp = sqlite3_column_int(stmt, 3);

        char *rawVols = (char *)sqlite3_column_text(stmt, 2);

        Record record = Record(id, espID, rawVols, timestamp);
        // std::cout<<record.getID()<<std::endl;
        result.push_back(record);

    }
   
    return result;

};

// Return -1 if throw error
int PIRDB::latestTimestamp() {
    char* query = "SELECT MAX(time) FROM pir;";
    sqlite3_stmt *stmt;
    int rc;
    rc = sqlite3_prepare_v2(this->db, query , -1, &stmt, 0);
    
    if (rc != SQLITE_OK) {
        std::cout<<"Cannot prepare statement: "<< sqlite3_errmsg(this->db)<<"\n";
        return -1;
    } 

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        return (int) sqlite3_column_int(stmt, 0);

    }
    return -1;

}

int PIRDB::oldestTimestamp(){
    char* query = "SELECT MIN(time) FROM pir;";
    sqlite3_stmt *stmt;
    int rc;
    rc = sqlite3_prepare_v2(this->db, query , -1, &stmt, 0);
    
    if (rc != SQLITE_OK) {
        std::cout<<"Cannot prepare statement: "<< sqlite3_errmsg(this->db)<<"\n";
        return -1;
    } 

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        return (int) sqlite3_column_int(stmt, 0);

    }
    return -1;

}

/* Implement class Record*/
Record::Record(int id, int espID, char *rawVol, int timestamp)
{
    this->_id = id;
    this->_espID = espID;
    this->_rawVol = std::string(rawVol);
    this->_timestamp = timestamp;
};

int Record::getID()
{
    return this->_id;
}

int Record::getEspID()
{
    return this->_espID;
}

int Record::getTimestamp()
{
    return this->_timestamp;
}

std::vector<int> Record::getVols()
{
    int chunk = 0;
    std::vector<int> result = {};

    for (char c : this->_rawVol)
    {
        if (c != SEPARATOR)
            chunk = chunk * 10 + (c - '0');
        else
        {
            result.push_back(chunk);
            chunk = 0;
        }
    }
    return result;
};

Json::Value Record::toJson()
{
    
    Json::Value root;

    // Guard NULL
    if (this->_id < 0) return root;

    root["id"] = this->_id;
    root["esp_id"] = this->_espID;
    root["timestamp"] = this->_timestamp;
    root["voltages"] = Json::arrayValue;
    std::vector<int> vols = this->getVols();

    for (int vol : vols)
    {
        root["voltages"].append(vol);
    };

    return root;
}

std::string Record::toJsonString()
{
    Json::Value root;
    root["id"] = this->_id;
    root["esp_id"] = this->_espID;
    root["timestamp"] = this->_timestamp;
    root["voltages"] = Json::arrayValue;
    std::vector<int> vols = this->getVols();

    for (int vol : vols)
    {
        root["voltages"].append(vol);
    };

    Json::FastWriter fastWriter;
    std::string output = fastWriter.write(root);

    return output;
}