//
// Created by Jesko Förster on 18.04.2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 5678
#define BUFFER_SIZE 1024

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "main.h"

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

        }else if(strcmp(method,"GET") == 0){
            const char* key = subarrays[1];
            const char* value = map_get_element(map, key);
            printf("Got: %s. The Value is: %s\n",key,value);

        }else if(strcmp(method,"DEL") == 0){
            const char* key = subarrays[1];
            map_delete_element(map, key);
            printf("Deleted: %s.\n",key);

        }else if(strcmp(method,"HELP") == 0){
            printf("<Help>\nPossible Commands: \"PUT\", \"GET\", \"DEL\", \"HELP\", \"QUIT\" \n</Help>\n");
        }else{
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


int create_socket() {
    int sockfd, newsockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    char buffer[BUFFER_SIZE];
    int buffer_pos = 0;

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Setup server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // Listen on any available interface
    server_addr.sin_port = htons(PORT);

    // Bind socket to address
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(sockfd, 5) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // Accept incoming connections
    client_len = sizeof(client_addr);
    if ((newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    printf("Client connected...\n");

    //write to view chars
    write(sockfd, "Yeet", 4);

    // Receive data from client and echo back
    ssize_t bytes_received;
    while ((bytes_received = recv(newsockfd, buffer + buffer_pos, BUFFER_SIZE - buffer_pos, 0)) > 0) {
        buffer_pos += bytes_received;

        // Check if a complete line has been received
        for (int i = 0; i < buffer_pos; ++i) {
            if (buffer[i] == '\n') {
                buffer[i] = '\0'; // Null-terminate the string at newline character
                printf("Client said: %s\n", buffer); // Print what client said

                // Check if client wants to quit
                if (strcmp(buffer, "QUIT\n") == 0) {
                    printf("Client requested to quit. Closing connection.\n");
                    close(newsockfd);
                    close(sockfd);
                    return 0; // Exit the program
                }

                // Move remaining data to the beginning of the buffer
                memmove(buffer, buffer + i + 1, buffer_pos - i - 1);
                buffer_pos -= i + 1;
                i = -1; // Restart from the beginning to check for more complete lines
            }
        }
    }

    if (bytes_received < 0) {
        perror("Receive failed");
    }

    // Close sockets
    close(newsockfd);
    close(sockfd);

    return 0;
}


