/**
 * File: dbmanager.c
 * 
 * Project: CLManager
 * 
 * Author: Issiah J Banda
 * 
 * Date Of Creation: 2023-10-05 //YYYY-MM-DD
 * 
 * Description: Used to abstract the interaction with the sqlite database from other
 *              parts of the project. This includes creating a connection, executing
 *              SQL commands, getting data, and clean up. Also makes handling operation
 *              errors simpler.
 * 
 * Modification History:
 *      - 2023-10-05: Worked on creating the foundational operations
 * 
*/

#include <stdio.h>

#include "sqlite3.h"
#include "dbmanager.h"

static sqlite3* db;

int makeConnection(void) {
    if (db != NULL) {
        // Only need to create connection once
        return OPERATION_SUCCESS;
    }
    
    // Open a db connection and check to make sure it worked
    int rc = sqlite3_open("data/testing.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return OPERATION_FAIL;
    }

    printf("Database Connected!!");
    createTable();
    return OPERATION_SUCCESS;
}

static void createTable(void) {
    char* errorMsg = 0;
    char* sqlStatement = "CREATE TABLE Books ("
                        "BookID INTEGER PRIMARY KEY,"
                        "Title TEXT NOT NULL,"
                        "Author TEXT NOT NULL,"
                        "Publisher TEXT,"
                        "PublicationData TEXT,"
                        "ISBN TEXT UNIQUE,"
                        "Genre TEXT,"
                        "Language TEXT,"
                        "NumberOfPages INTEGER"
                        ");";

    int rc = sqlite3_exec(db, sqlStatement, 0, 0, &errorMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Creating Table Error: %s\n", errorMsg);
        sqlite3_free(errorMsg);
        return;
    }
}

int closeConnection(void) {
    if (db == NULL) {
        return OPERATION_FAIL;
    }

    int rc = sqlite3_close(db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error deallocating database: %s\n", sqlite3_errmsg(db));
        return OPERATION_FAIL;
    }
    return OPERATION_SUCCESS;
}