#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "map.h"
#include "sub.h"

#define PORT 5678
#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024

typedef struct {
    int client_socket;
    Map *map;
} ClientData;

void *client_handler(void *arg) {
    ClientData *data = (ClientData *)arg;
    int client_socket = data->client_socket;
    Map *map = data->map;
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

            write(client_socket, full_input, input_length);
            char *result = handle_command(map, full_input);
            write(client_socket, result, strlen(result)); // Send result to client
            input_length = 0; // Reset input_length for next command
        }
    }

    // Close client socket
    close(client_socket);
    free(data); // Free dynamically allocated data
    pthread_exit(NULL);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    Map *map = map_create(); // Assuming map is initialized somewhere in your code
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

    printf("Server listening on port %d...\n", PORT);

    // Accept incoming connections and handle them
    while (1) {
        // Accept a client connection
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket < 0) {
            perror("Accept failed");
            continue; // Continue to wait for next connection
        }

        printf("New client connected\n");

        // Create data structure to pass to thread
        client_data = (ClientData *)malloc(sizeof(ClientData));
        if (client_data == NULL) {
            perror("Memory allocation failed");
            close(client_socket);
            continue; // Continue to wait for next connection
        }
        client_data->client_socket = client_socket;
        client_data->map = map;

        // Create thread to handle client
        if (pthread_create(&tid, NULL, client_handler, (void *)client_data) != 0) {
            perror("Thread creation failed");
            close(client_socket);
            free(client_data);
            continue; // Continue to wait for next connection
        }
    }

    // Close server socket
    close(server_socket);

    return 0;
}
