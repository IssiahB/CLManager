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
 *      - 2023-10-05: Worked on creating the database connect and close function.
 *                      Also defined the BookData struct.
 *      - 2023-10-07: Worked on creating addBook function.
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
    int rc = sqlite3_open("data/library.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return OPERATION_FAIL;
    }

    printf("Database Connected!!\n");
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
                        "PublicationDate TEXT,"
                        "ISBN TEXT UNIQUE CHECK (LENGTH(ISBN) = 13),"
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

int addBook(BookData data) {
    int rc = 0;
    const char* sqlInsert = "INSERT INTO Books (Title, Author, Publisher, PublicationDate, ISBN, Genre, Language, NumberOfPages) "
                            "VALUES (?, ?, ?, ?, ?, ?, ?, ?)";

    // Prepare sql statement
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sqlInsert, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL Error When Inserting Data: %s\n", sqlite3_errmsg(db));
        return OPERATION_FAIL;
    }

    // Bind values to sql statement
    sqlite3_bind_text(stmt, 1, data.title, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, data.author, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, data.publisher, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, data.publicationDate, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, data.ISBN, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, data.genre, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt,7, data.lang, -1, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 8, data.numPages);

    // Execute insert
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "SQL Error When Executing INSERT: %s\n", sqlite3_errmsg(db));
        return OPERATION_FAIL;
    }

    // TODO store id in some sort of structure. First construct structure
    sqlite3_int64 lastRowID = sqlite3_last_insert_rowid(db);
    printf("Data inserted successfully");
    sqlite3_finalize(stmt);
    return OPERATION_SUCCESS;
}

int removeBookById(int id) {
    int rc = 0;
    sqlite3_stmt* stmt;
    const char* sqlDelete = "DELETE FROM Books WHERE BookID = ?";

    rc = sqlite3_prepare_v2(db, sqlDelete, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL Error When Deleting Data: %s\n", sqlite3_errmsg(db));
        return OPERATION_FAIL;
    }

    // Bind id and execute sql
    sqlite3_bind_int(stmt, 1, id);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "SQL Error When Executing DELETE: %s\n", sqlite3_errmsg(db));
        return OPERATION_FAIL;
    }

    printf("Data deleted successfully");
    sqlite3_finalize(stmt);
    return OPERATION_SUCCESS;
}

int removeBookByTitle(char* title) {
    return 0;
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