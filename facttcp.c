#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 256

int factorial(int num) {
    if (num == 0 || num == 1) {
        return 1;
    }
    return num * factorial(num - 1);
}

int main()
{
    char BUFFER[BUFFER_SIZE];
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("Socket");
        exit(1);
    }

    struct sockaddr_in server_address, client_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        perror("Bind");
        exit(1);
    }
    if (listen(sockfd, 5) == -1)
    {
        perror("Listen");
        exit(1);
    }

    socklen_t addr_size = sizeof(client_address);

    int newsockfd = accept(sockfd, (struct sockaddr *)&client_address, &addr_size);
    if (newsockfd == -1)
    {
        perror("Accept");
        exit(1);
    }

    printf("Client connected!\n");
    while (1)
    {
        memset(BUFFER, 0, BUFFER_SIZE);
        int recvcheck = recv(newsockfd, BUFFER, BUFFER_SIZE, 0);
        if (recvcheck <= 0)
        {
            printf("Client disconnected.\n");
            break;
        }

        printf("Client: %s", BUFFER);
        if (strncmp(BUFFER, "exit", 4) == 0)
        {
            printf("Connection closed by client.\n");
            break;
        }

        int num = atoi(BUFFER);
        int result = factorial(num);
        snprintf(BUFFER, BUFFER_SIZE, "Factorial of %d is %d", num, result);
        send(newsockfd, BUFFER, strlen(BUFFER), 0);

        if (strncmp(BUFFER, "exit", 4) == 0)
        {
            printf("Connection closed by server.\n");
            break;
        }
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}
