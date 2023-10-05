#ifndef DBMANAGER_H
#define DBMANAGER_H

#define OPERATION_SUCCESS 1
#define OPERATION_FAIL 0

/**
 * Creates a connection to the sqlite database. Also ensures the
 * default tables are created.
 * @returns OPERATION_SUCCESS if connection was successful and
 *          returns OPERATION_FAIL if failed to connect.
*/
int makeConnection(void);

/**
 * Creates the default tables
*/
static void createTable(void);

/**
 * Closes the connection to the sqlite database.
 * @returns OPERATION_SUCCESS if close was successful else
 *          returns OPERATION_FAIL if the database connection is NULL
 *          or if the database is unable to deallocate.
*/
int closeConnection(void);

#endif