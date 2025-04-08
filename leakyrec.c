#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define NOF_PACKETS 10

int randNo(int a) {
    int rn = (rand() % 10) % a;
    return rn == 0 ? 1 : rn;  // Ensure non-zero random value
}

int main() {
    srand(time(NULL));  // Seed the random number generator
    int packet_sz[NOF_PACKETS], i, clk, b_size, o_rate;
    int p_sz_rm = 0, p_time, op;

    // Generate random packet sizes
    for (i = 0; i < NOF_PACKETS; ++i)
        packet_sz[i] = (rand()%10);  // Random packet size between 0 and 90

    // Display packet sizes
    for (i = 0; i < NOF_PACKETS; ++i)
        printf("\nPacket[%d]: %d bytes", i, packet_sz[i]);

    // Get output rate and bucket size
    printf("\n\nEnter the Output rate (bytes/sec): ");
    scanf("%d", &o_rate);

    printf("Enter the Bucket Size (bytes): ");
    scanf("%d", &b_size);

    // Process each packet
    for (i = 0; i < NOF_PACKETS; ++i) {
        if ((packet_sz[i] + p_sz_rm) > b_size) {
            if (packet_sz[i] > b_size) {
                printf("\n\nIncoming packet size (%d bytes) is greater than bucket capacity (%d bytes) - PACKET REJECTED", packet_sz[i], b_size);
            } else {
                printf("\n\nBucket capacity exceeded - PACKET REJECTED!");
            }
        } else {
            p_sz_rm += packet_sz[i];
            printf("\n\nIncoming Packet size: %d bytes", packet_sz[i]);
            printf("\nBytes remaining to transmit: %d bytes", p_sz_rm);

            p_time = (((rand()%10)+1)*10);
            printf("\nTime left for transmission: %d units", p_time);

            for (clk = 10; clk <= p_time; clk += 10) {
                sleep(1);
                if (p_sz_rm) {
                    if (p_sz_rm <= o_rate) {
                        op = p_sz_rm;
                        p_sz_rm = 0;
                    } else {
                        op = o_rate;
                        p_sz_rm -= o_rate;
                    }
                    printf("\nPacket of size %d bytes transmitted", op);
                    printf("\n----Bytes remaining to transmit: %d bytes", p_sz_rm);
                } else {
                    printf("\nTime left for transmission: %d units", p_time - clk);
                    printf("\nNo packets to transmit!!");
                }
            }
        }
    }

    return 0;
}
