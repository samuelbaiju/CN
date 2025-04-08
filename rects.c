#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 12347
#define BUFFER_SIZE 256

int peri(int len,int bred) {
    // Function to calculate the perimeter of a circle

    int peri = 2 * (len+bred) ;// Using 3.14 as an approximation for pi
    return peri;
}

int main() {
    char BUFFER1[BUFFER_SIZE];
    char BUFFER2[BUFFER_SIZE];
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
        int recvcheck1 = recvfrom(sockfd, BUFFER1, BUFFER_SIZE, 0, (struct sockaddr*)&client_address, &addr_size);
        int recvcheck2 = recvfrom(sockfd, BUFFER2, BUFFER_SIZE, 0, (struct sockaddr*)&client_address, &addr_size);
        if (recvcheck1 == -1) {
            perror("Recv failed");
            continue;
        }

        BUFFER1[recvcheck1] = '\0';
        BUFFER2[recvcheck2] = '\0';
        printf("length: %s\n", BUFFER1);
        printf("breadth: %s\n", BUFFER2);

        if (recvcheck2 == -1) {
            perror("Recv failed");
            continue;
        }

        
        

        if (strcmp(BUFFER1, "exit") == 0) {
            printf("Client disconnected. Waiting for new connections...\n");
            continue;
        }
       // char reply[BUFFER_SIZE];
        int len = atoi(BUFFER1); // Convert string to integer
        int bred = atoi(BUFFER2);
        int perimter= peri(len,bred); // Calculate perimeter
        sendto(sockfd,&perimter,sizeof(perimter),0,(struct sockaddr*)&client_address,addr_size);
        //snprintf(reply, BUFFER_SIZE, "The perimeter of the circle with radius %d is %d", radius, perimter);
        //sendto(sockfd,reply, strlen(reply), 0, (struct sockaddr*)&client_address, addr_size);
    }

    close(sockfd);
    return 0;
}
