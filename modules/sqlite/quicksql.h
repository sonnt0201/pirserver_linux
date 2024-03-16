#include "sqlite3.h"
#include <iostream>
#include <string>
#pragma once

#define SUCCESS 0
#define FAIL 1
#define ROW SQLITE_ROW
typedef sqlite3_stmt STATEMENT;
typedef sqlite3* DATABASE ;


inline int sql_prepare(sqlite3* &database, sqlite3_stmt** p_stmt, char* query_string)
{

    int rc = sqlite3_prepare_v2(database, query_string, -1, p_stmt, 0);
    if (rc != SQLITE_OK)
    {
        std::cout << "Error: Unable to prepare statement: "<<query_string << " - " 
                  << sqlite3_errmsg(database) << std::endl;
        return FAIL;
    }

    return SUCCESS;
}

// bind text value to statement, position starts with 1
inline int sql_bind_text(sqlite3* database, STATEMENT* &stmt, int position,  std::string value) {
   int  rc = sqlite3_bind_text(stmt, position , (char*) value.c_str(), -1, NULL);
    if (rc != SQLITE_OK) {
        std::cout<<"Error: Failed to bind. "<<sqlite3_errmsg(database)<<std::endl;
        return FAIL;
    }
    return SUCCESS;
}

// bind int 
inline int sql_bind_int(sqlite3* database, STATEMENT* stmt, int position, int value) {
    int rc = sqlite3_bind_int(stmt, position, value);
    if (rc != SQLITE_OK) {
        std::cout<<"Error: Failed to bind. "<<sqlite3_errmsg(database)<<std::endl;
        return FAIL;
    }

    return SUCCESS;
}

// step
inline int sql_step(STATEMENT* stmt) {
    int rc = sqlite3_step(stmt);
    return rc;
}
