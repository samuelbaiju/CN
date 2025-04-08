#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 256

int main()
{
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

    printf("Connected to server!\n");

    while (1)
    {
        printf("Client: ");
        fgets(BUFFER, BUFFER_SIZE, stdin);
        send(sockfd, BUFFER, strlen(BUFFER), 0);
        
        if (strncmp(BUFFER, "exit", 4) == 0)
        {
            printf("Connection closed by Client.\n");
            break;
        }

        memset(BUFFER, 0, BUFFER_SIZE);
        int recvcheck = recv(sockfd, BUFFER, BUFFER_SIZE, 0);
        if (recvcheck <= 0)
        {
            printf("Server disconnected.\n");
            break;
        }

        printf("Server: %s", BUFFER);

        if (strncmp(BUFFER, "exit", 4) == 0)
        {
            printf("Connection closed by server.\n");
            break;
        }
    }

    close(sockfd);
    return 0;
}
