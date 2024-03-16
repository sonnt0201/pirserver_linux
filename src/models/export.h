/*
    Models are mostly ORM for Sqlite3
    export.h include all definitions and exports of needed models
*/

#include "../../modules/export.h"


#pragma once

#define VOLNUM 100
#define SEPARATOR '_'

extern class PIR_ORM pirOrm;

class PIR_ORM
{
private:
    sqlite3 *_db;
    char* _filename;

public:

    PIR_ORM(char* filename);

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
    int createPIR(ID group, String description, ID* pir );


    /*
        Create a new record associated with a specific PIR.
        Return error code: SUCCESS or FAIL.
        [in] pir: ID of the PIR the record belongs to.
        [in] vol: Voltage reading as a String.
        [in] timestamp: Timestamp of the reading (defaults to current time).
    */
    int createRecord(ID pir, String vol, int timestamp);

    
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
        [out] pirs: vector containing the retrieved PIR objects.
    */
    std::vector<class PIR> readPIRs(ID group);

     /*
        Read the description of a group.
        [in] group: ID of the group to retrieve description for.
        [out] description: String containing the group's description.
    */
    String readGroupDescription(ID group);

    /*
        Read information about a specific PIR.
        [in] pir: ID of the PIR to retrieve information for.
        [out] pir: PIR object containing the retrieved information.
    */
    class PIR readPIR(ID pir);


    
};

/* TO-DO: Implement Request Row class */
class Record
{
private:
    int _id, _PIRID, _timestamp;
    std::string _rawVol;

public:
    Record(int id, int espID, char *rawVol, int timestamp);
    int getID();
    int getPIRID();
    std::vector<int> getVols();
    int getTimestamp();

    Json::Value toJson();

    std::string toJsonString();
    static std::string csvTitleRow();
    std::string toCsvRow();
};

class PIR {
    private:
    ID _id, _group;
    String _description;

    public: 
    inline PIR(ID id, ID group, String description){
        _id = id;
        _group = group;
        _description = description;
    };

    inline ID id() {
        return _id;
    }; 

    inline String description() {
        return _description;
    }

    
};