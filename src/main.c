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
 *      - 2023-10-16: Testing curl and it's functionallity
 * 
*/

#include <stdio.h>
#include <curl/curl.h>

#include "dbmanager.h"

void printCommands(void);

int main(void) {
    int oper = makeConnection();
    if (oper == OPERATION_FAIL) {
        return 1;
    }

    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        // curl_easy_setopt(curl, CURLOPT_CAINFO, "bin/cacert.pem");
        // curl_easy_setopt(curl, CURLOPT_URL, "https://openlibrary.org/search.json");
        // res = curl_easy_perform(curl);
        // if (res != CURLE_OK) {
        //     fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        // }
        curl_easy_cleanup(curl);
    }

    printCommands();
    closeConnection();

    return 0;
}

void printCommands(void) {
    printf("Commands:\n");
    printf(" s - Search for new books to add to collection\n");
    printf(" v - View books currently available in collection\n");
    printf(" x - Exit the program\n");
}