#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>

#define PORT 12347
#define BUFFER_SIZE 256

int main(){
    char BUFFER[BUFFER_SIZE];
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

    socklen_t addr_size = sizeof(server_address);

    while (1){
        printf("Client: ");
        fgets(BUFFER, BUFFER_SIZE, stdin);
        BUFFER[strcspn(BUFFER,"\n")]=0;
        sendto(sockfd,BUFFER,strlen(BUFFER),0,(struct sockaddr*)&server_address,sizeof(server_address));
        if (strcmp (BUFFER,"exit")==0){
            printf("client exiting...\n");
            break;
        }
        int recvcheck=recvfrom(sock_fd,BUFFER,BUFFER_SIZE,0,NULL,NULL);
        if (recvcheck == -1) {
            perror("Recv failed");
            break;
        }
        BUFFER[recvcheck] = '\0';
        printf("Server: %s\n",BUFFER);
    }
    close(sockfd);
    return 0;
}