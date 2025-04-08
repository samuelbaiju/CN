#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 12347
#define BUFFER_SIZE 256

int main() {
    char BUFFER[BUFFER_SIZE];
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address, client_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    socklen_t addr_size = sizeof(client_address);

    while (1) {
        int recvcheck = recvfrom(sockfd, BUFFER, BUFFER_SIZE, 0, (struct sockaddr*)&client_address, &addr_size);
        if (recvcheck == -1) {
            perror("Recv failed");
            continue;
        }

        BUFFER[recvcheck] = '\0';
        printf("Client: %s\n", BUFFER);

        if (strcmp(BUFFER, "exit") == 0) {
            printf("Client disconnected. Waiting for new connections...\n");
            continue;
        }

        printf("Server: ");
        fgets(BUFFER, BUFFER_SIZE, stdin);
        BUFFER[strcspn(BUFFER, "\n")] = 0;
        sendto(sockfd, BUFFER, strlen(BUFFER), 0, (struct sockaddr*)&client_address, addr_size);
    }

    close(sockfd);
    return 0;
}
