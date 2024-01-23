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
 *      - 2023-10-09: Finished the deleteBookById function and worked on getBooks function.
 *      - 2023-10-11: Finished the getBooks function along with creating BookArray struct.
 *                      Removed extra print statements, and added comments to functions.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sqlite3.h"
#include "dbmanager.h"

static sqlite3* db;
/**
 * Creates the default tables
*/
static void createTable(void);
/**
 * For allocating memory to dest, the same size as src with null-terminator. Will
 * print error to stderr if unable to allocate memory.
 * 
 * @param dest A pointer to a pointer to a character array. Memory will be allocated
 *              to this pointer, the size of the character array src.
 * @param src The source character array that contains the characters to be copied
 * @returns 1 if operation was successful, else returns 0.
*/
static int copyField(char** dest, const char* src);

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
        // Table is already created
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
    sqlite3_finalize(stmt);
    return OPERATION_SUCCESS;
}

int deleteBookById(int id) {
    if (id < 1) {
        return OPERATION_FAIL;
    }

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

    sqlite3_finalize(stmt);
    return OPERATION_SUCCESS;
}

BookArray getBooks(void) {
    int rc = 0;
    sqlite3_stmt* stmt;
    const char* sqlSelect = "SELECT * FROM Books";

    // Return this error result if error
    BookArray errorResult;
    errorResult.books = NULL;
    errorResult.count = -1;

    BookData** books = NULL;
    rc = sqlite3_prepare_v2(db, sqlSelect, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL Error: %s\n", sqlite3_errmsg(db));
        return errorResult;
    }

    int rowCount = 0;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        books = realloc(books, (rowCount + 1) * sizeof(BookData *));
        if (books == NULL) {
            fprintf(stderr, "Error Allocating Memory in getBooks for books\n");
            return errorResult;
        }

        books[rowCount] = malloc(sizeof(BookData));
        if (books[rowCount] == NULL) {
            fprintf(stderr, "Error Allocating Memory in getBooks for books of count\n");
            freeBooks(books, rowCount);
            return errorResult;
        }

        // Get book data from database and allocate memory for each field
        if (!copyField(&(books[rowCount]->title), sqlite3_column_text(stmt, 1)) ||
            !copyField(&(books[rowCount]->author), sqlite3_column_text(stmt, 2)) ||
            !copyField(&(books[rowCount]->publisher), sqlite3_column_text(stmt, 3)) ||
            !copyField(&(books[rowCount]->publicationDate), sqlite3_column_text(stmt, 4)) ||
            !copyField(&(books[rowCount]->ISBN), sqlite3_column_text(stmt, 5)) ||
            !copyField(&(books[rowCount]->genre), sqlite3_column_text(stmt, 6)) ||
            !copyField(&(books[rowCount]->lang), sqlite3_column_text(stmt, 7))) {
            fprintf(stderr, "Memory Allocation Error: %s\n", sqlite3_errmsg(db));
            freeBooks(books, rowCount + 1); // rowCount plus one for including this row
            return errorResult;
        }

        books[rowCount]->numPages = sqlite3_column_int(stmt, 8);
        
        rowCount++;
    }

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Error In getBooks(): %s\n", sqlite3_errmsg(db));
        freeBooks(books, rowCount);
        return errorResult;
    }

    sqlite3_finalize(stmt);
    BookArray result;
    result.books = books;
    result.count = rowCount;
    return result;
}

static int copyField(char** dest, const char* src) {
    *dest = malloc(strlen(src) + 1); // Plus one for null-terminator
    if (*dest == NULL) {
        fprintf(stderr, "Error Allocating Memory in getBooks\n");
        return 0; // Return failure
    }
    // Copies database data into field
    strcpy(*dest, src);
    return 1; // Return success
}

void freeBooks(BookData** books, int numBooks) {
    for (int i = 0; i < numBooks; i++) {
        free(books[i]->title);
        free(books[i]->author);
        free(books[i]->publisher);
        free(books[i]->publicationDate);
        free(books[i]->ISBN);
        free(books[i]->genre);
        free(books[i]->lang);
        free(books[i]);
    }
    free(books);
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