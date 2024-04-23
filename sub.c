//
// Created by Jesko Förster on 18.04.2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "main.h"

#define PORT 5678
#define BUFFER_SIZE 1024
#define ENDLOSSCHLEIFE 1


int create_key_value_socket(Map *map){
    int rfd; // Rendevouz-Descriptor
    int cfd; // Verbindungs-Descriptor

    struct sockaddr_in client; // Socketadresse eines Clients
    socklen_t client_len; // Länge der Client-Daten
    char in[BUFFER_SIZE]; // Daten vom Client an den Server
    int bytes_read; // Anzahl der Bytes, die der Client geschickt hat


    // Socket erstellen
    rfd = socket(AF_INET, SOCK_STREAM, 0);
    if (rfd < 0 ){
        fprintf(stderr, "socket konnte nicht erstellt werden\n");
        exit(-1);
    }


    // Socket Optionen setzen für schnelles wiederholtes Binden der Adresse
    int option = 1;
    setsockopt(rfd, SOL_SOCKET, SO_REUSEADDR, (const void *) &option, sizeof(int));


    // Socket binden
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);
    int brt = bind(rfd, (struct sockaddr *) &server, sizeof(server));
    if (brt < 0 ){
        fprintf(stderr, "socket konnte nicht gebunden werden\n");
        exit(-1);
    }


    // Socket lauschen lassen
    int lrt = listen(rfd, 5);
    if (lrt < 0 ){
        fprintf(stderr, "socket konnte nicht listen gesetzt werden\n");
        exit(-1);
    }

    printf("Created Socket\nWaiting for input...\n");

    while (ENDLOSSCHLEIFE) {
        cfd = accept(rfd, (struct sockaddr *) &client, &client_len);

        write(cfd, "Moegliche Befehle: GET, DEL, PUT, QUIT\n\r", 40);

        bytes_read = read(cfd, in, BUFFER_SIZE);

        char full_input[BUFFER_SIZE];
        int input_length = 0;

        while (bytes_read > 0) {
            // Daten dem full_input hinzufügen
            strncpy(full_input + input_length, in, bytes_read);
            input_length += bytes_read;

            // Überprüfen, ob das letzte Zeichen ein Enter ist
            if (in[bytes_read - 1] == '\n') {
                // Remove trailing "\r\n" characters
                if (input_length >= 2 && full_input[input_length - 2] == '\r' && full_input[input_length - 1] == '\n') {
                    full_input[input_length - 2] = '\0'; // Replace '\r' with '\0'
                    full_input[input_length - 1] = '\0'; // Replace '\n' with '\0'
                    input_length -= 2; // Adjust input length
                }

                //print command and handel command
                printf("Received complete input from client: %s\n", full_input);

                write(cfd, full_input, input_length);
                char* result = handle_command(map, full_input);
                //strcat(result, "\r \n");
                write(cfd, result,sizeof(result));
                input_length = 0;
            }

            bytes_read = read(cfd, in, BUFFER_SIZE);
        }
        close(cfd);
    }
}

char** splitByWhiteSpace(const char *longArray, int* numSubarrays) {
    char** subarrays = (char**)malloc(1024 * sizeof(char*)); // Allocate memory for subarrays
    if (!subarrays) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    char *token;
    char *rest = strdup(longArray); // Duplicate the input string to avoid modifying it
    if (!rest) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    int count = 0; // Counter for subarrays

    // Tokenize the input string and store subarrays
    const char splitChar = ' '; // Split only by space character
    while ((token = strtok_r(rest, &splitChar, &rest))) {
        subarrays[count] = strdup(token);
        if (!subarrays[count]) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
        count++;
    }

    *numSubarrays = count; // Update numSubarrays with the count
    return subarrays;
}

char* handle_command(Map *map, const char *command) {

    //split the command
    int numSubarrays;
    char** subarrays = splitByWhiteSpace(command, &numSubarrays);
    const char* method = subarrays[0];

    if(strcmp(method,"QUIT") == 0){
        printf("Quiting CLI.\n");

        //return value
        char buffer[100]; // Assuming a fixed buffer size for simplicity, adjust as needed
        sprintf(buffer, "Quitting Server\n");
        char* result = malloc(strlen(buffer) + 1); // Allocate memory for the string
        strcpy(result, buffer); // Copy the formatted string into the allocated memory
        return result;
    }
    else if(strcmp(method,"PUT") == 0){
        const char* key = subarrays[1];
        const char* value = subarrays[2];
        map_insert_element(map, key, value);
        printf("Inserted: %s and %s.\n",key,value);

        //return value
        char buffer[100]; // Assuming a fixed buffer size for simplicity, adjust as needed
        sprintf(buffer, "Inserted %s with the value %s\n", key, value);
        char* result = malloc(strlen(buffer) + 1); // Allocate memory for the string
        strcpy(result, buffer); // Copy the formatted string into the allocated memory
        return result;
    }
    else if(strcmp(method,"GET") == 0){
        const char* key = subarrays[1];
        const char* value = map_get_element(map, key);
        printf("Got: %s. The Value is: %s\n",key,value);

        //return value
        char buffer[100]; // Assuming a fixed buffer size for simplicity, adjust as needed
        sprintf(buffer, "Got %s with it has the value %s\n", key, value);
        char* result = malloc(strlen(buffer) + 1); // Allocate memory for the string
        strcpy(result, buffer); // Copy the formatted string into the allocated memory
        return result;
    }
    else if(strcmp(method,"DEL") == 0)
    {
        const char* key = subarrays[1];
        map_delete_element(map, key);
        printf("Deleted: %s.\n",key);

        //return value
        char buffer[100]; // Assuming a fixed buffer size for simplicity, adjust as needed
        sprintf(buffer, "Deleted %s form the Map\n", key);
        char* result = malloc(strlen(buffer) + 1); // Allocate memory for the string
        strcpy(result, buffer); // Copy the formatted string into the allocated memory
        return result;
    }
    else if(strcmp(method,"BEG") == 0){
        printf("Beginning for transaction.\n");
        printf("Yet to implement.\n");

        //return value
        char buffer[100]; // Assuming a fixed buffer size for simplicity, adjust as needed
        sprintf(buffer, "Starting exclusive connection to Map\n");
        char* result = malloc(strlen(buffer) + 1); // Allocate memory for the string
        strcpy(result, buffer); // Copy the formatted string into the allocated memory
        return result;

    }
    else if(strcmp(method,"END") == 0){
        printf("Ending for transaction.\n");

        //return value
        char buffer[100]; // Assuming a fixed buffer size for simplicity, adjust as needed
        sprintf(buffer, "Starting exclusive connection to Map\n");
        char* result = malloc(strlen(buffer) + 1); // Allocate memory for the string
        strcpy(result, buffer); // Copy the formatted string into the allocated memory
        return result;

    }
    else if(strcmp(method,"HELP") == 0){
        printf("<Help>\nPossible Commands: \"PUT\", \"GET\", \"DEL\", \"HELP\", \"QUIT\", \"BEG\", \"END\" \n</Help>\n");

        //return value
        char buffer[100]; // Assuming a fixed buffer size for simplicity, adjust as needed
        sprintf(buffer, "<Help>\nPossible Commands: \"PUT\", \"GET\", \"DEL\", \"HELP\", \"QUIT\", \"BEG\", \"END\" \n</Help>\n");
        char* result = malloc(strlen(buffer) + 1); // Allocate memory for the string
        strcpy(result, buffer); // Copy the formatted string into the allocated memory
        return result;
    }
    else{
        //stuck?
        printf("Command not found, please enter a valid command. Enter \"HELP\" to so see possible commands.");

        //return value
        char buffer[100]; // Assuming a fixed buffer size for simplicity, adjust as needed
        sprintf(buffer, "Command not found, please enter a valid command. Enter \"HELP\" to so see possible commands.");
        char* result = malloc(strlen(buffer) + 1); // Allocate memory for the string
        strcpy(result, buffer); // Copy the formatted string into the allocated memory
        return result;
    }
}
