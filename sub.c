//
// Created by Jesko Förster on 18.04.2024.
//

#include "sub.h"

#define BUFFER_SIZE 1024

void handle_client(int client_socket, Map *map) {
    char in[BUFFER_SIZE];
    int bytes_read;
    char full_input[BUFFER_SIZE];
    int input_length = 0;

    // Send initial message to client
    writeConnectionMessage(client_socket);

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

/**
 * Writes an overview of available commands to the client socket.
 * @param client_socket The socket to write to.
 */
void writeConnectionMessage(int client_socket) {
    char overview[1024] = "Available commands:\n\r"
                         "HELP -> Prints this information\n\r"
                         "GET <key> -> Prints value for the specified key\n\r"
                         "PUT <key> <value> -> Stores the specified key-value pair\n\r"
                         "DEL <key> -> Deletes the specified key\n\r"
                         "QUIT -> Exits the program\n\r"
                         "BEG -> Begins a transaction\n\r"
                         "END -> Ends a transaction\n\r"
                         "\n";

    write(client_socket, overview, strlen(overview));
}

char** splitByWhiteSpace(const char* command, int* numSubarrays) {
    // Find the index of the first uppercase letter
    const char* ptr = command;
    while (*ptr && !isupper(*ptr)) ptr++;

    // If no uppercase letter is found in the entire command
    if (*ptr == '\0') {
        *numSubarrays = 1;
        char** result = (char**)malloc(sizeof(char*));
        result[0] = (char*)malloc(strlen("notFound") + 1);
        strcpy(result[0], "notFound");
        return result;
    }

    // Count the number of whitespaces starting from the first uppercase letter
    int count = 0;
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

    // Split the command at whitespaces starting from the first uppercase letter
    int index = 0;
    ptr = command;
    while (*ptr && !isupper(*ptr)) ptr++;
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
        sprintf(buffer, "Ending exclusive connection to Map\n\r");
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
        printf("Command not found, please enter a valid command. Enter \"HELP\" to so see possible commands.\n\r");

        //return value
        char buffer[100]; // Assuming a fixed buffer size for simplicity, adjust as needed
        sprintf(buffer, "Command not found, please enter a valid command. Enter \"HELP\" to so see possible commands.\n\r");
        char* result = malloc(strlen(buffer) + 1); // Allocate memory for the string
        strcpy(result, buffer); // Copy the formatted string into the allocated memory
        return result;
    }
}

Map * createSharedMemoryMap(){
    // Erstellen des Shared Memory
    int shmid = shmget(IPC_PRIVATE, sizeof(Map), IPC_CREAT|0600);
    if (shmid == -1) {
        perror("shmget failed");
        exit(EXIT_FAILURE);
    }

    // Anhängen des Shared Memory an den Prozess
    Map * shar_mem_map = (Map *)shmat(shmid, 0, 0);
    if (shar_mem_map == (void *)-1) {
        perror("shmat failed");
        exit(EXIT_FAILURE);
    }

    return shar_mem_map;
}