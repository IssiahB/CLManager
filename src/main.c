/**
 * File: main.c
 * 
 * Project: CLManager
 * 
 * Author: Issiah J Banda
 * 
 * Date Of Creation: 2023-10-05 //YYYY-MM-DD
 * 
 * Description: The main entry point for the project. Handles the main
 *              looping of the project.
 * 
 * Modification History:
 *      - 2023-10-05: Testing out database operations in the main function
 * 
*/

#include <stdio.h>

#include "dbmanager.h"

int main(void) {
    int oper = makeConnection();
    if (oper == OPERATION_FAIL) {
        return 1;
    }

    closeConnection();

    return 0;
}