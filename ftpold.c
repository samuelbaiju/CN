#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 256
FILE *fp;

int main()
{
    char filename[100];
    char BUFFER[BUFFER_SIZE];
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("Socket");
        exit(1);
    }

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP Address

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        perror("Connect");
        exit(1);
    }

    printf("Connected to CLIENT!\n");

    recv(sockfd, filename, sizeof(filename), 0);
    printf("File name received: %s\n", filename);

    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        perror("File");
        exit(1);
    }
    else {
        while(fgets(BUFFER, BUFFER_SIZE, fp) != NULL){
            send(sockfd, BUFFER, sizeof(BUFFER), 0);
        }
    }

    close(sockfd);
    return 0;
}
