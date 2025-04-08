#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 12340
#define BUFFER_SIZE 256

int main()
{
    char buffer[BUFFER_SIZE];
    char filename[20];

    // Create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        perror("connect");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Get filename from user and send to server
    printf("Enter the filename to request: ");
    scanf("%s", filename);
    send(sockfd, filename, strlen(filename), 0);
    
    // Receive and print file content
    printf("Receiving file content:\n");
    while (1){
        int recvcheck = recv(sockfd, buffer, BUFFER_SIZE - 1, 0);
        if (recvcheck <= 0)
            break;

        buffer[recvcheck] = '\0';
        printf("%s", buffer);
    }

    printf("\nFile received successfully.\n");
    close(sockfd);
    return 0;
}
