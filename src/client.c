#include "client.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUFFER_SIZE 1024

uint64_t SERVER_ADDRESS;
int32_t CLIENT_SOCKET;

void start_client(uint64_t server_address) {
    SERVER_ADDRESS = server_address;

    // Create socket
    int32_t client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    CLIENT_SOCKET = client_socket;

    // Set up server address
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_address & 0xFFFF);  // Extract port from server address
    server_addr.sin_addr.s_addr = htonl(server_address >> 16);  // Extract IP from server address

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection to server failed");
        exit(EXIT_FAILURE);
    }

    handle_server(client_socket);

    terminate_client();
}

void handle_server(int32_t ssocket) {
    // Implement server communication logic here

}

int receive_file(char *directory, int socket) {
    char filename[256];  // Adjust the buffer size according to your needs

    // Get the filename from the client
    ssize_t bytes_received = recv(socket, filename, sizeof(filename) - 1, 0);

    if (bytes_received <= 0) {
        perror("Error receiving filename");
        return -1;
    }

    // Construct the full path of the file
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "%s/%s", directory, filename);

    // Open the file for writing
    int fd = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd == -1) {
        perror("Error opening file for writing");
        return -1;
    }

    // Receive and write file data
    char buffer[1024];
    ssize_t bytes_read;
    while ((bytes_read = recv(socket, buffer, sizeof(buffer), 0)) > 0) {
        write(fd, buffer, bytes_read);
    }

    // Close the file
    close(fd);

    return 0;
}

int send_file(char *directory, int socket) {
    char filename[256];

    // Get the filename to send from the client
    ssize_t bytes_received = recv(socket, filename, sizeof(filename) - 1, 0);

    if (bytes_received <= 0) {
        perror("Error receiving filename");
        return -1;
    }

    // Construct the full path of the file
    char filepath[256]; 
    snprintf(filepath, sizeof(filepath), "%s/%s", directory, filename);

    // Open the file for reading
    int fd = open(filepath, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file for reading");
        return -1;
    }

    // Read and send file data
    char buffer[1024];
    ssize_t bytes_read;
    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        send(socket, buffer, bytes_read, 0);
    }

    // Close the file
    close(fd);

    return 0;
}

void terminate_client() {
    close(CLIENT_SOCKET);
    // Perform any additional cleanup if needed
}