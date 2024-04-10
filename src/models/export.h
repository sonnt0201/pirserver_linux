/*
    Models are mostly ORM for Sqlite3
    export.h include all definitions and exports of needed models
*/

#include "../../modules/export.h"

#pragma once

#define VOLNUM 100
#define SEPARATOR '+'

extern class PIR_ORM pirOrm;

class PIR_ORM
{
private:
    sqlite3 *_db;
    char *_filename;

public:
    PIR_ORM(char *filename);

    /*
        Create a new user and assign a unique user token (ID).
        Return error code: SUCCESS or FAIL.
        [out] user_token: pointer to variable receiving the generated user ID.
    */
    int createUser(ID *user_token);

    /*
        Check if user_token is valid
    */
    bool validUser(ID user_token);

    /*
        Create a new group of PIRs
        Return error code : SUCCESS or FAIL.
        [in] description: Description in String type for group.
        [out] group: pointer to variable receiving the group ID
    */
    int createGroup(String description, ID *group);

    bool isGroupExists(ID group);

    /*
       Create a new PIR (Passive Infrared sensor) within a group.
       Return error code: SUCCESS or FAIL.
       [in] group: ID of the group to which the PIR belongs.
       [in] description: Description in String type for the PIR.
       [out] pir: pointer to variable receiving the PIR ID.
   */
    int createPIR(ID group, String description, ID *pir);

    /*
        Create a new record associated with a specific PIR.
        Return error code: SUCCESS or FAIL.
        [in] pir: ID of the PIR the record belongs to.
        [in] vol: Voltage reading as a String.
        [in] timestamp: Timestamp of the reading (defaults to current time).
    */
    int createRecord(ID pir, String vol, int timestamp);

    std::vector<ID> readPIRsOfGroup(ID group);

    /*
        Read a list of records within a specified group and time range.
        [in] group: ID of the group to query records from.
        [in] begin: Starting timestamp for the query.
        [in] end: Ending timestamp for the query (defaults to current time).
        [out] records: vector containing the retrieved Record objects.
    */
    std::vector<class Record> readRecords(ID group, int begin, int end = time(NULL));

    /*
        Read all PIRs associated with a specific group.
        [in] group: ID of the group to query PIRs from.
        [return] pirs: vector containing the retrieved PIR objects.
    */
    std::vector<class PIR> readPIRs(ID group);

    /*
       Read the description of a group.
       [in] group: ID of the group to retrieve description for.
       [out] description: String containing the group's description.
   */
    int readGroupDescription(ID group, String *description);

    /*
        Read information about a specific PIR.
        [in] pir_id: ID of the PIR to retrieve information for.
        [out] pir: PIR object containing the retrieved information.
    */
    int readPIR(ID pir_id, class PIR *pir);

    /*
        Read information about a specific User.
        [in] user_token: ID of the User to retrieve information for.
        [out] user: User object containing the retrieved information.
    */
    int readUser(ID user_token, class User *user);

    std::vector<class PirGroup> readAllGroups();
};

/* TO-DO: Implement Request Row class */
class Record
{
private:
    ID _id, _PIRID;
    int _timestamp;
    std::string _rawVol;

public:
    Record(char *recordId, char *pirID, char *rawVol, int timestamp);
    ID getID();
    ID getPIRID();
    std::vector<int> getVols();
    int getTimestamp();

    Json::Value toJson();

    std::string toJsonString();
    static std::string csvTitleRow();
    std::string toCsvRow();
};

class PIR
{
private:
    ID _id, _group;
    String _description;

public:
    inline PIR(ID id, ID group, String description)
    {
        _id = id;
        _group = group;
        _description = description;
    };

    inline ID id()
    {
        return _id;
    };

    inline String description()
    {
        return _description;
    }

    inline JSON toJson()
    {
        JSON json;
        json["pir_id"] = _id;
        json["group_id"] = _group;
        json["pir_description"] = _description;

        return json;
    }
};

class User
{
private:
    String _token, _name, _description;

public:
    inline User(String token, String name, String description)
    {
        _token = token;
        _name = name;
        _description = description;
    };

    inline ID token() { return _token; }
    inline String name() { return _name; }
    inline String description() { return _description; }

    inline JSON toJson() {
        JSON json;
        json["user_token"] = _token;
        json["name"] = _name;
        json["description"] = _description;
        return json;
    }
};

class PirGroup {
    private:
    ID _id;
    String _description;

    public:
    inline PirGroup(ID id, String description) {
        _id = id;
        _description = description;
    }

    inline ID id() { return _id; }
    inline String description() { return _description; }

    inline JSON toJson() {
        JSON root;
        root["id"] = _id;
        root["description"] = _description;
        return root;
    }
};