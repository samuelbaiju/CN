#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 12340
#define BUFFER_SIZE 256

int main()
{
    char buff[BUFFER_SIZE];
    char filename[20];

    // Create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address, client_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, 5) == -1)
    {
        perror("listen");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);
    socklen_t addr_size = sizeof(client_address);
    int newsockfd = accept(sockfd, (struct sockaddr *)&client_address, &addr_size);
    if (newsockfd == -1)
    {
        perror("accept");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Client connected.\n");

    // Receive filename from client
    int recvcheck = recv(newsockfd, filename, sizeof(filename) - 1, 0);
    if (recvcheck <= 0)
    {
        perror("recv filename");
        close(newsockfd);
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    filename[recvcheck] = '\0';

    printf("Client requested file: %s\n", filename);

    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        strcpy(buff, "File not found on server.\n");
        send(newsockfd, buff, strlen(buff), 0);
    }
    else {
        while (fgets(buff, BUFFER_SIZE, fp) != NULL) {
            send(newsockfd, buff, strlen(buff), 0);
        }
        fclose(fp);
    }

    printf("File transfer complete.\n");

    close(newsockfd);
    close(sockfd);
    return 0;
}
