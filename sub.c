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

char** splitByChar(char longArray[1024], int* numSubarrays, const char* splittChar) {
    char** subarrays = (char**)malloc(1024 * sizeof(char*)); // Allocate memory for subarrays
    if (!subarrays) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    char *token;
    char *rest = longArray;
    int count = 0; // Counter for subarrays

    // Tokenize the input string and store subarrays
    while ((token = strtok_r(rest, splittChar, &rest))) {
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

int create_command_line_interface() {
    //set up map
    Map *map = map_create();

    //take first command
    printf("Welcome to the server please enter a command!\n");
    char command[1024];
    scanf(" %[^\n]", command);

    //split the command
    int numSubarrays;
    const char splitter =':';
    char** subarrays = splitByChar(command, &numSubarrays, &splitter);
    const char* method = subarrays[0];

    //loop until "QUIT" is entered
    while(strcmp(method,"QUIT") != 0){

        if(strcmp(method,"PUT") == 0){
            const char* key = subarrays[1];
            const char* value = subarrays[2];
            map_insert_element(map, key, value);
            printf("Inserted: %s and %s.\n",key,value);
        }
        else if(strcmp(method,"GET") == 0){
            const char* key = subarrays[1];
            const char* value = map_get_element(map, key);
            printf("Got: %s. The Value is: %s\n",key,value);
        }
        else if(strcmp(method,"DEL") == 0)
        {
            const char* key = subarrays[1];
            map_delete_element(map, key);
            printf("Deleted: %s.\n",key);

        }
        else if(strcmp(method,"BEG") == 0){
            printf("Beginning for transaction.\n");
            printf("Yet to implement.\n");

        }
        else if(strcmp(method,"END") == 0){
            printf("Ending for transaction.\n");
            printf("Yet to implement.\n");

        }
        else if(strcmp(method,"HELP") == 0){
            printf("<Help>\nPossible Commands: \"PUT\", \"GET\", \"DEL\", \"HELP\", \"QUIT\", \"BEG\", \"END\" \n</Help>\n");
        }
        else{
            printf("Command not found, please enter a valid command. Enter \"HELP\" to so see possible commands.");
        }

        //get new command
        printf("Please enter a new command. \n");
        scanf("%s",&command);

        //split the new command
        subarrays = splitByChar(command, &numSubarrays, &splitter);
        method = subarrays[0];
    }

    map_destroy(map);
    //Show that the program has ended
    printf("Main terminated, because \"QUIT\" was entered.\n");
    return 0;
}


int create_socket_nils(){
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
        write(cfd, "Einabe: GET [key]\n\r", 19);
        write(cfd, "Einabe: DEL [key]\n\r", 19);
        write(cfd, "Einabe: PUT [key] [value]\n\r\n", 28);

        bytes_read = read(cfd, in, BUFFER_SIZE);

        char full_input[BUFFER_SIZE];
        int input_length = 0;

        while (bytes_read > 0) {
            // Daten dem full_input hinzufügen
            strncpy(full_input + input_length, in, bytes_read);
            input_length += bytes_read;

            // Überprüfen, ob das letzte Zeichen ein Enter ist
            if (in[bytes_read - 1] == '\n') {
                printf("Received complete input from client: %s", full_input);
                write(cfd, full_input, input_length);
                input_length = 0;
            }

            bytes_read = read(cfd, in, BUFFER_SIZE);
        }
        close(cfd);
    }
}
