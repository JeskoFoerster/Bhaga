#include "map.h"
#include "sub.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define PORT 5678
#define MAX_CLIENTS 5


int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

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
            handle_client(client_socket, shar_mem_map);

            // Close client socket and exit child process
            close(client_socket);
            exit(EXIT_SUCCESS);
        } else {
            // Parent process
            close(client_socket); // Close client socket in parent process
        }
    }
}
