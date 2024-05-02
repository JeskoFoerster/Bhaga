#include "main.h"

#define PORT 5678
#define MAX_CLIENTS 5

int main() {

    bool inTransaction = false;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    int shm_id = shmget(IPC_PRIVATE, MAX_CLIENTS * sizeof(int), IPC_CREAT | 0600);
    int *msg_q_ids = (int *)shmat(shm_id, 0, 0);
    for(int i = 0; i < MAX_CLIENTS; i++){
        msg_q_ids[i] = 0;
    }

    // Create server socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
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

    Map * shar_mem_map = createSharedMemoryMap();

    int sem_group_id = semaphore_create_group(1);
    if (sem_group_id == -1) {
        perror ("Die Gruppe konnte nicht angelegt werden!");
        exit(EXIT_FAILURE);
    }

    // Anschließend wird der Semaphor auf 1 gesetzt
    semaphore_set_value(sem_group_id, 0, 1);

    printf("Server listening on port %d...\n", PORT);

    int client_socket;
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

            // Anlegen einer Nachrichtenschlange
            int msg_q_id = messageQueueCreate();
            if (msg_q_id < 0) {
                perror("Fehler beim erstellen der Nachrichtenschlange!");
                exit(1);
            }

            // Each client gets their own queue
            int client_count = 0;
            while(client_count < MAX_CLIENTS && msg_q_ids[client_count] != 0){
                client_count++;
            }
            msg_q_ids[client_count] = msg_q_id;

            // Child process
            close(server_socket); // Close server socket in child process

            // Handle client in child process
            handle_client(client_socket, shar_mem_map, sem_group_id, &inTransaction, msg_q_id, msg_q_ids);

        } else {
            // Parent process
            close(client_socket); // Close client socket in parent process
        }
    }
}
