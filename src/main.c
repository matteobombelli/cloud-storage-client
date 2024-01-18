#include "client.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc != 1)
    {
        printf("Usage: ./client\n");
        exit(EXIT_FAILURE);
    }
    
    uint32_t server_ip;
    printf("Enter server ip:\n");
    scanf("%" SCNu32 "\n", &server_ip);

    start_client(server_ip);

    exit(EXIT_SUCCESS);
}