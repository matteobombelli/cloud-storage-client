#ifndef CLIENT_H
#define CLIENT_H

#include <stdint.h>

// Starts the client and tries connecting to server_ip
void start_client(uint64_t server_address);

// Handles communication with the server
void handle_server(int32_t socket);

// Copies a given file to the user's directory
// RETURNS 0 on success, -1 on failure
int receive_file(char *directory, int socket);

// Sends a given file to the client
// RETURNS 0 on success, -1 on failure
int send_file(char *directory, int socket);

// Terminates the client and frees all memory
void terminate_client(void);

#endif /* CLIENT_H */