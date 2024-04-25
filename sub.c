//
// Created by Jesko Förster on 18.04.2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#include "main.h"

#define PORT 5678
#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024

typedef struct {
    int client_socket;
    Map *map;
} ClientData;

int create_key_value_socket(Map *map){
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    pthread_t tid;
    ClientData *client_data;

    // Create server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize server address struct
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket to port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    //Shared Memory:
    int shmid;
    Map *shar_mem;

    // Erstellen des Shared Memory
    shmid = shmget(IPC_PRIVATE, sizeof(Map), IPC_CREAT|0600);
    if (shmid == -1) {
        perror("shmget failed");
        exit(EXIT_FAILURE);
    }

    // Anhängen des Shared Memory an den Prozess
    shar_mem = (Map *)shmat(shmid, 0, 0);
    if (shar_mem == (void *)-1) {
        perror("shmat failed");
        exit(EXIT_FAILURE);
    }

    // Kopieren der Map-Daten in den Shared Memory
    memcpy(shar_mem, map, sizeof(*map));

    printf("Server listening on port %d...\n", PORT);

    // Accept incoming connections and handle them
    while (1) {
        // Accept a client connection
        client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &client_len);
        if (client_socket < 0) {
            perror("Accept failed");
            continue; // Continue to wait for next connection
        }

        printf("New client connected\n");

        // Fork a new process
        pid_t pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            close(client_socket);
            continue; // Continue to wait for next connection
        }

        if (pid == 0) {
            // Child process
            close(server_socket); // Close server socket in child process

            // Handle client in child process
            handle_client(client_socket, shar_mem);

            // Close client socket and exit child process
            close(client_socket);
            exit(EXIT_SUCCESS);
        } else {
            // Parent process
            close(client_socket); // Close client socket in parent process
        }
    }
}

void handle_client(int client_socket, Map *map) {
    char in[BUFFER_SIZE];
    int bytes_read;
    char full_input[BUFFER_SIZE];
    int input_length = 0;

    // Send initial message to client
    write(client_socket, "Moegliche Befehle: GET, DEL, PUT, QUIT\n\r", 40);

    // Receive data from client and process it
    while ((bytes_read = read(client_socket, in, BUFFER_SIZE)) > 0) {
        // Add received data to full_input
        strncpy(full_input + input_length, in, bytes_read);
        input_length += bytes_read;

        // Check if the last character is a newline
        if (in[bytes_read - 1] == '\n') {
            // Remove trailing "\r\n" characters
            if (input_length >= 2 && full_input[input_length - 2] == '\r' && full_input[input_length - 1] == '\n') {
                full_input[input_length - 2] = '\0'; // Replace '\r' with '\0'
                full_input[input_length - 1] = '\0'; // Replace '\n' with '\0'
                input_length -= 2; // Adjust input length
            }

            // Print received command and handle it
            printf("Received complete input from client: %s\n", full_input);

            // Write response to client
            char *result = handle_command(map, full_input);
            //destroy socket if demanded
            if (strcmp(result,"QUIT") == 0) {
                write(client_socket, "Connection closed!", 18);
                close(client_socket);
            }
            write(client_socket, result, strlen(result)); // Send result to client
            free(result); // Free dynamically allocated result
            input_length = 0; // Reset input_length for next command
        }
    }

    // Close client socket
    close(client_socket);
}

char** splitByWhiteSpace(const char* command, int* numSubarrays) {
    // Count the number of whitespaces to determine the size of the result array
    int count = 0;
    const char* ptr = command;
    while (*ptr) {
        // Skip leading whitespaces
        while (*ptr && *ptr == ' ') ptr++;
        if (*ptr) {
            count++;
            // Skip the current word
            while (*ptr && *ptr != ' ') ptr++;
        }
    }

    // Allocate memory for the result array
    char** result = (char**)malloc(count * sizeof(char*));
    if (!result) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    // Split the command at whitespaces and store the parts in the result array
    int index = 0;
    ptr = command;
    while (*ptr) {
        // Skip leading whitespaces
        while (*ptr && *ptr == ' ') ptr++;
        if (*ptr) {
            const char* start = ptr;
            // Move to the end of the current word
            while (*ptr && *ptr != ' ') ptr++;
            const char* end = ptr;
            int length = end - start;
            // Allocate memory for the current part and copy it
            result[index] = (char*)malloc((length + 1) * sizeof(char)); // +1 for the null terminator
            if (!result[index]) {
                fprintf(stderr, "Error: Memory allocation failed.\n");
                exit(EXIT_FAILURE);
            }
            strncpy(result[index], start, length);
            result[index][length] = '\0'; // Add null terminator
            index++;
        }
    }

    *numSubarrays = count;
    return result;
}


char* handle_command(Map *map, const char *command) {

    //split the command
    int numSubarrays;
    char** subarrays = splitByWhiteSpace(command, &numSubarrays);
    const char* method = subarrays[0];

    if(strcmp(method,"QUIT") == 0){
        printf("Quiting CLI.\n\r");
        //return value
        char* result = "QUIT";
        return result;
    }
    else if(strcmp(method,"PUT") == 0){
        const char* key = subarrays[1];
        const char* value = subarrays[2];
        map_insert_element(map, key, value);
        printf("Inserted: %s and %s.\n\r",key,value);

        //return value
        char buffer[100]; // Assuming a fixed buffer size for simplicity, adjust as needed
        sprintf(buffer, "Inserted %s with the value %s\n\r", key, value);
        char* result = malloc(strlen(buffer) + 1); // Allocate memory for the string
        strcpy(result, buffer); // Copy the formatted string into the allocated memory
        return result;
    }
    else if(strcmp(method,"GET") == 0){
        const char* key = subarrays[1];
        const char* value = map_get_element(map, key);
        printf("Got: %s. The Value is: %s\n\r",key,value);

        //return value
        char buffer[100]; // Assuming a fixed buffer size for simplicity, adjust as needed
        sprintf(buffer, "Got %s with it has the value %s\n\r", key, value);
        char* result = malloc(strlen(buffer) + 1); // Allocate memory for the string
        strcpy(result, buffer); // Copy the formatted string into the allocated memory
        return result;
    }
    else if(strcmp(method,"DEL") == 0)
    {
        const char* key = subarrays[1];
        map_delete_element(map, key);
        printf("Deleted: %s.\n\r",key);

        //return value
        char buffer[100]; // Assuming a fixed buffer size for simplicity, adjust as needed
        sprintf(buffer, "Deleted %s form the Map\n\r", key);
        char* result = malloc(strlen(buffer) + 1); // Allocate memory for the string
        strcpy(result, buffer); // Copy the formatted string into the allocated memory
        return result;
    }
    else if(strcmp(method,"BEG") == 0){
        printf("Beginning for transaction.\n\r");
        printf("Yet to implement.\n\r");

        //return value
        char buffer[100]; // Assuming a fixed buffer size for simplicity, adjust as needed
        sprintf(buffer, "Starting exclusive connection to Map\n\r");
        char* result = malloc(strlen(buffer) + 1); // Allocate memory for the string
        strcpy(result, buffer); // Copy the formatted string into the allocated memory
        return result;

    }
    else if(strcmp(method,"END") == 0){
        printf("Ending for transaction.\n\r");

        //return value
        char buffer[100]; // Assuming a fixed buffer size for simplicity, adjust as needed
        sprintf(buffer, "Starting exclusive connection to Map\n\r");
        char* result = malloc(strlen(buffer) + 1); // Allocate memory for the string
        strcpy(result, buffer); // Copy the formatted string into the allocated memory
        return result;

    }
    else if(strcmp(method,"HELP") == 0){
        printf("<Help>\n\rPossible Commands: \"PUT\", \"GET\", \"DEL\", \"HELP\", \"QUIT\", \"BEG\", \"END\" \n\r</Help>\n\r");

        //return value
        char buffer[100]; // Assuming a fixed buffer size for simplicity, adjust as needed
        sprintf(buffer, "<Help>\n\rPossible Commands: \"PUT\", \"GET\", \"DEL\", \"HELP\", \"QUIT\", \"BEG\", \"END\" \n\r</Help>\n\r");
        char* result = malloc(strlen(buffer) + 1); // Allocate memory for the string
        strcpy(result, buffer); // Copy the formatted string into the allocated memory
        return result;
    }
    else{
        //stuck?
        printf("Command not found, please enter a valid command. Enter \"HELP\" to so see possible commands.\n\r");

        //return value
        char buffer[100]; // Assuming a fixed buffer size for simplicity, adjust as needed
        sprintf(buffer, "Command not found, please enter a valid command. Enter \"HELP\" to so see possible commands.\n\r");
        char* result = malloc(strlen(buffer) + 1); // Allocate memory for the string
        strcpy(result, buffer); // Copy the formatted string into the allocated memory
        return result;
    }
}
