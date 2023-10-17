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

int main(void) {
    int oper = makeConnection();
    if (oper == OPERATION_FAIL) {
        return 1;
    }

    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_CAINFO, "bin/curl-ca-bundle.crt");
        curl_easy_setopt(curl, CURLOPT_URL, "https://www.google.com");
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);
    }

    
    closeConnection();

    return 0;
}