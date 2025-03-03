//
// Created by yuvali027 on 5/29/24.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define PORT 8080
#define BUFFER_SIZE 1048576 // 1MB

double get_time_in_seconds() {
    struct timeval time;
    gettimeofday(&time, NULL);
    return time.tv_sec + time.tv_usec / 1000000.0;
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char *buffer = malloc(BUFFER_SIZE);
    double start_time, end_time;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        return -1;
    }

    for (int size = 1; size <= BUFFER_SIZE; size *= 2) {
        memset(buffer, 'a', size);
        int bytes_sent = 0;
        int bytes_written;

        start_time = get_time_in_seconds();
        while (bytes_sent < BUFFER_SIZE) {
            bytes_written = write(sock, buffer, size);
            if (bytes_written < 0) {
                perror("write");
                exit(EXIT_FAILURE);
            }
            bytes_sent += bytes_written;
        }
        end_time = get_time_in_seconds();

        double throughput = (bytes_sent * 8.0) / (end_time - start_time) / (1024 * 1024); // Mbps
        printf("Sent %d bytes in %f seconds. Throughput: %f Mbps\n", bytes_sent, end_time - start_time, throughput);
    }

    close(sock);
    free(buffer);
    return 0;
}
