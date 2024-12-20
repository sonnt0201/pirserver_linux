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
        "CREATE TABLE IF NOT EXISTS Records (record_id VARCHAR PRIMARY KEY, pir_id VARCHAR NOT NULL, vol VARCHAR NOT NULL, time UNSIGNED BIG INT NOT NULL, FOREIGN KEY (pir_id) REFERENCES PIRs(pir_id));",
    };

    sqlite3_stmt *stmt;

    for (auto query : creatingQueries)
    {
        rc = sqlite3_prepare_v2(_db, query, -1, &stmt, 0);

        if (rc != SQLITE_OK)
        {
            std::cout << "Error: Unable to prepare statement in PIR_ORM initialization - "
                      << sqlite3_errmsg(_db) << std::endl;
        }

        rc = sqlite3_step(stmt);

        rc = sqlite3_finalize(stmt);

        std::cout << "Initialize database successfully." << std::endl;
    }

    return;
}

bool PIR_ORM::validUser(ID user_token)
{
    char *query = "SELECT * FROM Users WHERE (user_token = ?);";

    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(_db, query, -1, &stmt, 0);

    if (rc != SQLITE_OK)
    {
        std::cout << "Error: Check valid user failed - " << sqlite3_errmsg(_db) << std::endl;
        return FAIL;
    }

    rc = sqlite3_bind_text(stmt, 1, (char *)user_token.c_str(), -1, NULL);

    rc = sqlite3_step(stmt);

    if (rc == SQLITE_ROW)
        return true;
    return false;
}

int PIR_ORM::createGroup(String description, ID *group)
{
    if (!this->_allowDataWrite) {
        std::cout<<"Data writing is not allowed.\n";
        return FAIL;
    } 

    char *query = "INSERT INTO PIRGroups VALUES (?, ?);";
    sqlite3_stmt *stmt;
    int rc;

    // prepare
    rc = sqlite3_prepare_v2(_db, query, -1, &stmt, 0);

    if (rc != SQLITE_OK)
    {
        std::cout << "Error: Create group failed - " << sqlite3_errmsg(_db) << std::endl;
        return FAIL;
    }

    // bind
    ID newgroup = uuid_v4();

    sqlite3_bind_text(stmt, 1, (char *)newgroup.c_str(), -1, NULL);
    sqlite3_bind_text(stmt, 2, (char *)description.c_str(), -1, NULL);

    if (rc != SQLITE_OK)
    {
        std::cout << "Error: Bind failed - " << sqlite3_errmsg(_db) << std::endl;
        return FAIL;
    }

    // execute
    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE)
    {
        std::cout << "Error: Execution failed - " << sqlite3_errmsg(_db) << std::endl;
        return FAIL;
    }

    *group = newgroup;
    return SUCCESS;
}

bool PIR_ORM::isGroupExists(ID group)
{
    char *query = "SELECT * FROM PIRGroups WHERE (pir_group = ?);";

    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(_db, query, -1, &stmt, 0);

    if (rc != SQLITE_OK)
    {
        std::cout << "Error: Check valid user failed - " << sqlite3_errmsg(_db) << std::endl;
        return FAIL;
    }

    rc = sqlite3_bind_text(stmt, 1, (char *)group.c_str(), -1, NULL);

    rc = sqlite3_step(stmt);

    if (rc == SQLITE_ROW)
        return true;
    return false;
}

int PIR_ORM::createPIR(ID group, String description, ID *pir)
{
    if (!this->_allowDataWrite) {
        std::cout<<"Data writing is not allowed.\n";
        return FAIL;
    } 

    char *query = "INSERT INTO PIRs VALUES (?, ?, ?);";
    sqlite3_stmt *stmt;
    int rc;

    // prepare
    rc = sqlite3_prepare_v2(_db, query, -1, &stmt, 0);

    if (rc != SQLITE_OK)
    {
        std::cout << "Error: Create group failed - " << sqlite3_errmsg(_db) << std::endl;
        return FAIL;
    }

    // bind
    ID newpir = uuid_v4();

    sqlite3_bind_text(stmt, 1, (char *)newpir.c_str(), -1, NULL);
    sqlite3_bind_text(stmt, 2, (char *)group.c_str(), -1, NULL);
    sqlite3_bind_text(stmt, 3, (char *)description.c_str(), -1, NULL);

    if (rc != SQLITE_OK)
    {
        std::cout << "Error: Bind failed - " << sqlite3_errmsg(_db) << std::endl;
        return FAIL;
    }

    // execute
    rc = sqlite3_step(stmt);

    std::cout << "Creating PIR: " << sqlite3_errmsg(_db) << std::endl;

    if (rc != SQLITE_DONE)
    {
        std::cout << "Error: Execution failed - " << sqlite3_errmsg(_db) << std::endl;
        return FAIL;
    }

    *pir = newpir;
    return SUCCESS;
}

int PIR_ORM::createRecord(ID pir, String vol, uint64_t timestamp)
{
      if (!this->_allowDataWrite) {
        std::cout<<"Data writing is not allowed.\n";
        return FAIL;
    } 

    char *query = "INSERT INTO Records(record_id, pir_id, vol, time) VALUES ( ?, ?, ?, ? );";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(_db, query, -1, &stmt, 0);

    if (rc != SQLITE_OK)
    {
        std::cout << "Error: Unable to prepare statement: " << query << " - "
                  << sqlite3_errmsg(_db) << std::endl;
        return FAIL;
    }

    ID newid = uuid_v4();

    sqlite3_bind_text(stmt, 1, (char *)newid.c_str(), -1, NULL);

    sqlite3_bind_text(stmt, 2, (char *)pir.c_str(), -1, NULL);
    sqlite3_bind_text(stmt, 3, (char *)vol.c_str(), -1, NULL);
    sqlite3_bind_int64(stmt, 4, timestamp);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {

        std::cout << "Error: Failed to execute. " << sqlite3_errmsg(_db) << std::endl;
        return FAIL;
    }

    return SUCCESS;
}

std::vector<ID> PIR_ORM::readPIRsOfGroup(ID group)
{
    std::vector<ID> results = {};
    char *query = "SELECT (pir_id) FROM PIRs WHERE (pir_group = ?)";
    sqlite3_stmt *stmt;
    int rc;

    rc = sqlite3_prepare_v2(this->_db, query, -1, &stmt, 0);
    if (rc != SQLITE_OK)
    {
        std::cout << "Error: Failed to prepare - " << sqlite3_errmsg(this->_db) << "\n";
        return results;
    }

    sqlite3_bind_text(stmt, 1, (char *)group.c_str(), -1, NULL);

    while (
        (rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        char *pirID_CString = (char *)sqlite3_column_text(stmt, 1);
        ID pirID(pirID_CString);
        results.push_back(pirID);
    }

    return results;
}

std::vector<class Record> PIR_ORM::readRecords(ID group, uint64_t begin, uint64_t end)
{

    std::vector<class Record> results = {};

    char *query = "SELECT * FROM Records WHERE ("
                  "pir_id IN (SELECT pir_id FROM PIRs WHERE (pir_group = ?))"
                  "AND time BETWEEN ? AND ?)";

    sqlite3_stmt *stmt;
    int rc;

    rc = sqlite3_prepare_v2(this->_db, query, -1, &stmt, 0);
    if (rc != SQLITE_OK)
    {
        std::cout << "Error: Failed to prepare - " << sqlite3_errmsg(this->_db) << "\n";
        return results;
    }

    sqlite3_bind_text(stmt, 1, (char *)group.c_str(), -1, NULL);
    sqlite3_bind_int64(stmt, 2, begin);
    sqlite3_bind_int64(stmt, 3, end);
    while (
        (rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        char *recordId = (char *)sqlite3_column_text(stmt, 0);
        // std::cout<<"record id: "<<recordId<<std::endl;
        char *pirId = (char *)sqlite3_column_text(stmt, 1);
        char *rawVol = (char *)sqlite3_column_text(stmt, 2);
        uint64_t timestamp = sqlite3_column_int64(stmt, 3);
        // std::cout<<"Timestamp from db: "<<timestamp<<std::endl;
        Record record = Record(recordId, pirId, rawVol, timestamp);

        results.push_back(record);
    }

    return results;
}

std::vector<class PirGroup> PIR_ORM::readAllGroups()
{
    std::vector<class PirGroup> results = {};

    char *query = "SELECT * FROM PIRGroups;";

    sqlite3_stmt *stmt;
    int rc;

    rc = sqlite3_prepare_v2(this->_db, query, -1, &stmt, 0);
    if (rc != SQLITE_OK)
    {
        std::cout << "Error: Failed to prepare - " << sqlite3_errmsg(this->_db) << "\n";
        return results;
    }

    while (
        (rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        char *id = (char *)sqlite3_column_text(stmt, 0);
        // std::cout<<"record id: "<<recordId<<std::endl;
        char *des = (char *)sqlite3_column_text(stmt, 1);

        PirGroup group = PirGroup(id, des);

        results.push_back(group);
    }

    return results;
};

/* Implement class Record*/
Record::Record(char *recordId, char *pirID, char *rawVol, uint64_t timestamp)
{
    this->_id = String(recordId);
    this->_PIRID = String(pirID);
    this->_rawVol = std::string(rawVol);
    this->_timestamp = timestamp;
};

ID Record::getID()
{
    return this->_id;
}

ID Record::getPIRID()
{
    return this->_PIRID;
}

uint64_t Record::getTimestamp()
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

    if (chunk > 0)result.push_back(chunk);
    
    return result;
};

Json::Value Record::toJson()
{

    Json::Value root;

    // Guard NULL
    if (this->_id == "")
        return root;

    root["record_id"] = this->_id;
    root["pir_id"] = this->_PIRID;
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

int PIR_ORM::readPIR(ID pir_id, class PIR *pir)
{
    char *query = "SELECT * FROM PIRs WHERE (pir_id = ? )";

    sqlite3_stmt *stmt;
    int rc = sql_prepare(_db, &stmt, query);
    // int rc = sqlite3_prepare_v2(this->_db, query, -1, &stmt, 0);

    if (rc != SQLITE_OK)
        return FAIL;

    // rc = sql_bind_text(_db, &stmt, 1, pir_id);
    sqlite3_bind_text(stmt, 1, (char *)pir_id.c_str(), -1, NULL);

    if (rc != SQLITE_OK)
        return FAIL;

    rc = sqlite3_step(stmt);

    if (rc == SQLITE_ROW)
    {

        ID pirId(
            (char *)sqlite3_column_text(stmt, 0));

        ID groupId(
            (char *)sqlite3_column_text(stmt, 1));
        String description(
            (char *)sqlite3_column_text(stmt, 2));

        *pir = PIR(
            pirId,
            groupId,
            description);

        return SUCCESS;
    }

    std::cout << "ERROR: Failed to execute statement. \n"
              << sqlite3_errmsg(_db) << std::endl;
    ;
    return FAIL;
}

int PIR_ORM::readUser(ID user_token, class User *user)
{
    char *query = "SELECT * FROM Users WHERE (user_token = ? )";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(this->_db, query, -1, &stmt, 0);

    if (rc != SQLITE_OK)
        return FAIL;

    // rc = sql_bind_text(_db, &stmt, 1, pir_id);
    sqlite3_bind_text(stmt, 1, (char *)user_token.c_str(), -1, NULL);

    if (rc != SQLITE_OK)
        return FAIL;

    rc = sqlite3_step(stmt);

    if (rc == SQLITE_ROW)
    {

        ID token(
            (char *)sqlite3_column_text(stmt, 0));

        String name(
            (char *)sqlite3_column_text(stmt, 1));
        String description(
            (char *)sqlite3_column_text(stmt, 2));

        *user = User(
            token,
            name,
            description);

        return SUCCESS;
    }

    std::cout << "ERROR: Failed to execute statement. \n"
              << sqlite3_errmsg(_db) << std::endl;
    ;
    return FAIL;
}

std::vector<class PIR> PIR_ORM::readPIRs(ID group)
{
    std::vector<class PIR> out = {};
    char *query = "SELECT * FROM PIRs WHERE pir_group = ?";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(this->_db, query, -1, &stmt, 0);

    if (rc != SQLITE_OK)
        return out;

    // rc = sql_bind_text(_db, &stmt, 1, pir_id);
    sqlite3_bind_text(stmt, 1, (char *)group.c_str(), -1, NULL);

    if (rc != SQLITE_OK)
        return out;

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {

        ID pirId(
            (char *)sqlite3_column_text(stmt, 0));

        String pirGroup(
            (char *)sqlite3_column_text(stmt, 1));
        String description(
            (char *)sqlite3_column_text(stmt, 2));

        PIR pir = PIR(pirId, pirGroup, description);
        out.push_back(pir);
    }

    std::cout
        << sqlite3_errmsg(_db) << std::endl;
    // ;
    return out;
}

std::vector<class Record> PIR_ORM::latestRecords(int num, ID groupID)
{
    char *query = "SELECT * FROM Records WHERE (pir_id IN (SELECT pir_id FROM PIRs WHERE (pir_group=?))) ORDER BY time DESC LIMIT ( (SELECT COUNT (*) FROM PIRs WHERE (pir_group=? )) * ? ) ";

    std::vector<class Record> out = {};

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(this->_db, query, -1, &stmt, 0);

    if (rc != SQLITE_OK){
        std::cout<<"Prepare failed: "
        <<sqlite3_errmsg(_db) << std::endl;
        return out;
    }

    // rc = sql_bind_text(_db, &stmt, 1, pir_id);
    sqlite3_bind_text(stmt, 1, (char *)groupID.c_str(), -1, NULL);
    sqlite3_bind_text(stmt, 2, (char *)groupID.c_str(), -1, NULL);
    sqlite3_bind_int(stmt, 3, num);

    // rc = sqlite3_step(stmt);
    
  while (
        (rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        char *recordId = (char *)sqlite3_column_text(stmt, 0);
        // std::cout<<"record id: "<<recordId<<std::endl;
        char *pirId = (char *)sqlite3_column_text(stmt, 1);
        char *rawVol = (char *)sqlite3_column_text(stmt, 2);
        uint64_t timestamp = sqlite3_column_int64(stmt, 3);

        Record record = Record(recordId, pirId, rawVol, timestamp);

        out.push_back(record);
    }

    return out;
};