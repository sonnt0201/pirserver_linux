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
                                 "id INTEGER PRIMARY KEY,"
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
        std::cout << "Query failed with id  "<< ID << ": " << sqlite3_errmsg(db)  << std::endl;
    }

    // Finalize the statement
    sqlite3_finalize(stmt);

    return result;
}

int PIRDB::addData(int deviceID, std::string vol, int time)
{
    auto start = std::chrono::high_resolution_clock::now();
    const char *query = "INSERT INTO pir (id, esp_id, vol, time)"
                        "VALUES (?, ?, ? , ?);";

    sqlite3_stmt *stmt;

    int rc;

    rc = sqlite3_prepare_v2(this->db, query, -1, &stmt, 0);

    if (rc != SQLITE_OK)
    {
        std::cerr << "Cannot prepare the statement: " << sqlite3_errmsg(this->db) << std::endl;
    }
    // set next id
    int nextID = this->numOfRows() + 1;
    sqlite3_bind_int(stmt, 1, nextID);
    sqlite3_bind_int(stmt, 2, deviceID);
    sqlite3_bind_text(stmt, 3, strdup(vol.c_str()), -1, NULL);

    sqlite3_bind_int(stmt, 4, time);

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

/* TO-DO: Implement method recordWithID() */ 
class RecordRow PIRDB::recordWithID(int ID) {
    /* Code goes here */
};

/* TO-DO: Implement class RecordRow */ 
RecordRow::RecordRow(int id, int espID, int timestamp, int voltage[VOLNUM]){
    /* Code goes here */
};

int RecordRow::id(){
    /* Code goes here */
    return this->_id;
};
int RecordRow::espID(){
    /* Code goes here */
};
int RecordRow::timestamp(){
    /* Code goes here */
};
std::vector<int> RecordRow::voltage(){
    /* Code goes here */
};
