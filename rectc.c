#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 12347
#define BUFFER_SIZE 256

int main() {
    char BUFFER1[BUFFER_SIZE];
    char BUFFER2[BUFFER_SIZE];
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY; // Keeping INADDR_ANY
    int perim;
    char BUFFER[BUFFER_SIZE];
    socklen_t addr_size = sizeof(server_address);

    while (1) {
        printf("length: ");
        fgets(BUFFER1, BUFFER_SIZE, stdin);
        
        printf("breadth: ");
        fgets(BUFFER2, BUFFER_SIZE, stdin);
        BUFFER1[strcspn(BUFFER1, "\n")] = 0;

        BUFFER2[strcspn(BUFFER2, "\n")] = 0;
        
        sendto(sockfd, BUFFER1, strlen(BUFFER1), 0, (struct sockaddr*)&server_address, addr_size);
        if (strcmp(BUFFER1, "exit") == 0) {
            printf("Client exiting...\n");
            break;
        }
        
            sendto(sockfd, BUFFER2, strlen(BUFFER2), 0, (struct sockaddr*)&server_address, addr_size);
            if (strcmp(BUFFER2, "exit") == 0) {
                printf("Client exiting...\n");
                break;
        }
        int recvcheck = recvfrom(sockfd, &perim, sizeof(perim), 0, NULL, NULL);
        if (recvcheck == -1) {
            perror("Recv failed");
            break;
        }
       // BUFFER1[recvcheck] = '\0';
        printf("perimeter: %d\n", perim);
    }

    close(sockfd);
    return 0;
}

