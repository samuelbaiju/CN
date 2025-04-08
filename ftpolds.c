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
    char filename[100];
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
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

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

    printf("server connected!\n");

    printf("Enter the file name: ");
    scanf("%s", filename);
    send(newsockfd, filename, sizeof(filename), 0);

    while(1){
        int recvcheck = recv(newsockfd, BUFFER, BUFFER_SIZE, 0);
        if (recvcheck <= 0)
        {
            printf("SERVER disconnected.\n");
            break;
        }
        if (recvcheck == -1){
            perror("Recv failed");
            break;
        }

        printf("%s\n", BUFFER);
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}
