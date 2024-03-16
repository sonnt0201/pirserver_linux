#include "export.h"

class PIR_ORM pirOrm = PIR_ORM("pir-dev.db");

// Implement orm
PIR_ORM::PIR_ORM(char *filename)
{
    this->_filename = filename;
    int rc = sqlite3_open(_filename, &_db);

    if (rc != SQLITE_OK)
    {
        std::cout << "Error: Cannnot open " << _filename << "\n"
                  << "In PIR_ORM initialization." << std::endl;
    }

    std::vector<const char *> creatingQueries = {
    "CREATE TABLE IF NOT EXISTS Users (user_token VARCHAR PRIMARY KEY, name VARCHAR, description VARCHAR);",
     "CREATE TABLE IF NOT EXISTS PIRGroups (pir_group VARCHAR PRIMARY KEY, description VARCHAR);",
    "CREATE TABLE IF NOT EXISTS PIRs (pir_id VARCHAR PRIMARY KEY, pir_group VARCHAR NOT NULL, description VARCHAR, FOREIGN KEY (pir_group) REFERENCES PIRGroups(pir_group));",
    "CREATE TABLE IF NOT EXISTS Records (record_id VARCHAR PRIMARY KEY, pir_id VARCHAR NOT NULL, vol VARCHAR NOT NULL, time INTEGER NOT NULL, FOREIGN KEY (pir_id) REFERENCES PIRs(pir_id));",
     };


    sqlite3_stmt *stmt;

    for (auto query : creatingQueries)
    {
        rc = sqlite3_prepare_v2(_db, query, -1, &stmt, 0);

        if (rc != SQLITE_OK)
        {
            std::cout << "Error: Unable to prepare statement in PIR_ORM initialization - "
            <<sqlite3_errmsg(_db) << std::endl;
        }

        rc = sqlite3_step(stmt);

        rc = sqlite3_finalize(stmt);

        std::cout << "Initialize database successfully." << std::endl;
    }

    return;
}

bool PIR_ORM::validUser(ID user_token) {
    char* query = "SELECT * FROM Users WHERE (user_token = ?);";

    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(_db, query, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        std::cout<<"Error: Check valid user failed - "<< sqlite3_errmsg(_db) << std::endl;
        return FAIL;
    }

    rc = sqlite3_bind_text(stmt, 1,(char*) user_token.c_str(), -1, NULL);

    rc = sqlite3_step(stmt);

    if (rc == SQLITE_ROW) return true;
    return false;
}

int PIR_ORM::createGroup(String description, ID *group) {
    char* query = "INSERT INTO PIRGroups VALUES (?, ?);";
    sqlite3_stmt* stmt;
    int rc;

    //prepare
    rc = sqlite3_prepare_v2(_db, query, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        std::cout<<"Error: Create group failed - "<< sqlite3_errmsg(_db) << std::endl;
        return FAIL;
    }

    // bind
    ID newgroup = uuid_v4();

    sqlite3_bind_text(stmt, 1, (char*) newgroup.c_str(), -1, NULL);
    sqlite3_bind_text(stmt, 2, (char*) description.c_str(), -1, NULL);

    if (rc != SQLITE_OK) {
        std::cout<<"Error: Bind failed - "<< sqlite3_errmsg(_db) << std::endl;
        return FAIL;
    }

    // execute
    rc = sqlite3_step(stmt);
    
    if (rc != SQLITE_DONE) {
        std::cout<<"Error: Execution failed - "<< sqlite3_errmsg(_db) << std::endl;
        return FAIL;
    }

    *group = newgroup;
    return SUCCESS;

}

bool PIR_ORM::isGroupExists(ID group){
     char* query = "SELECT * FROM PIRGroups WHERE (pir_group = ?);";

    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(_db, query, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        std::cout<<"Error: Check valid user failed - "<< sqlite3_errmsg(_db) << std::endl;
        return FAIL;
    }

    rc = sqlite3_bind_text(stmt, 1,(char*) group.c_str(), -1, NULL);

    rc = sqlite3_step(stmt);

    if (rc == SQLITE_ROW) return true;
    return false;
}

int PIR_ORM::createPIR(ID group, String description, ID* pir ) {
     char* query = "INSERT INTO PIRs VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;
    int rc;

    //prepare
    rc = sqlite3_prepare_v2(_db, query, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        std::cout<<"Error: Create group failed - "<< sqlite3_errmsg(_db) << std::endl;
        return FAIL;
    }

    // bind
    ID newpir = uuid_v4();

    sqlite3_bind_text(stmt, 1, (char*) newpir.c_str(), -1, NULL);
    sqlite3_bind_text(stmt, 2, (char*) group.c_str(), -1, NULL);
    sqlite3_bind_text(stmt, 3, (char*) description.c_str(), -1, NULL);

    if (rc != SQLITE_OK) {
        std::cout<<"Error: Bind failed - "<< sqlite3_errmsg(_db) << std::endl;
        return FAIL;
    }

    // execute
    rc = sqlite3_step(stmt);
    
     std::cout<<"Creating PIR: "<< sqlite3_errmsg(_db) << std::endl;
        

    if (rc != SQLITE_DONE) {
        std::cout<<"Error: Execution failed - "<< sqlite3_errmsg(_db) << std::endl;
        return FAIL;
    }

    *pir = newpir;
    return SUCCESS;
}

int PIR_ORM::createRecord(ID pir, String vol, int timestamp) {
   char* query = "INSERT INTO Records(record_id, pir_id, vol, time) VALUES ( ?, ?, ?, ? );";
   sqlite3_stmt * stmt;

   int rc = sqlite3_prepare_v2(_db, query, -1, &stmt, 0);

    if (rc != SQLITE_OK)
    {
        std::cout << "Error: Unable to prepare statement: "<<query << " - " 
                  << sqlite3_errmsg(_db) << std::endl;
        return FAIL;
    }

    ID newid = uuid_v4();

    sqlite3_bind_text(stmt, 1 , (char*) newid.c_str(), -1, NULL);


     sqlite3_bind_text(stmt, 2 , (char*) pir.c_str(), -1, NULL);
    sqlite3_bind_text(stmt,3, (char*)vol.c_str(), -1, NULL);
    sqlite3_bind_int(stmt, 4, timestamp);
   
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
      
        std::cout<<"Error: Failed to execute. "<<sqlite3_errmsg(_db)<<std::endl;
        return FAIL;
   
    }

    return SUCCESS;


}



/* Implement class Record*/
Record::Record(int id, int espID, char *rawVol, int timestamp)
{
    this->_id = id;
    this->_PIRID = espID;
    this->_rawVol = std::string(rawVol);
    this->_timestamp = timestamp;
};

int Record::getID()
{
    return this->_id;
}

int Record::getPIRID()
{
    return this->_PIRID;
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
    if (this->_id < 0)
        return root;

    root["id"] = this->_id;
    root["esp_id"] = this->_PIRID;
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
    root["esp_id"] = this->_PIRID;
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
};
std::string Record::csvTitleRow()
{
    std::stringstream ss;
    ss.clear();
    ss.str("");
    ss << "id,pir_id,timestamp";
    for (int i = 0; i < VOLNUM; i++)
        ss << "," << i;
    // ss<<"\n";
    return ss.str();
}
std::string Record::toCsvRow()
{
    std::stringstream ss;
    ss << this->_id << "," << this->_PIRID << "," << this->_timestamp;
    std::vector<int> vols = this->getVols();

    for (int i = 0; i < vols.size(); i++)
        ss << "," << vols[i];
    // ss<<"\n";
    return ss.str();
}