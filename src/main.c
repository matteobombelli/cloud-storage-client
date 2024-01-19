#include "client.h"
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

uint64_t ip_port_to_int(const char *ip_port) {
    // Split the IP and port
    char ip[32], port[32];
    if (sscanf(ip_port, "%[^:]:%s", ip, port) != 2) {
        fprintf(stderr, "Invalid IP:Port format\n");
        exit(EXIT_FAILURE);
    }

    // Convert IP address components to integers
    uint64_t ip_int = 0;
    char *token = strtok(ip, ".");
    while (token != NULL) {
        ip_int = (ip_int << 8) | atoi(token);
        token = strtok(NULL, ".");
    }

    // Convert port to integer
    uint16_t port_int = atoi(port);

    // Combine IP and port into a single integer
    return ((ip_int << 16) & 0xFFFFFFFFFFFF0000) | (port_int & 0xFFFF);
}

int main(int argc, char *argv[]) {
    if (argc != 1) {
        printf("Usage: ./client\n");
        exit(EXIT_FAILURE);
    }

    // Get server ip
    char server_ip_port[32];
    printf("Enter server ip (i.e. 123.456.78.901:23456): ");
    scanf("%s", server_ip_port);

    uint64_t server_address = ip_port_to_int(server_ip_port);

    start_client(server_address);

    exit(EXIT_SUCCESS);
}