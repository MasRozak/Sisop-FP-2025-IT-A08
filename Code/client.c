#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define IP "127.0.0.1"
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, IP, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    printf("Menghubungkan ke server ROT13 di %s:%d...\n", IP, PORT);
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }
    printf("Berhasil terhubung! Anda bisa mulai mengetik.\n");
    printf("Ketik 'exit' untuk keluar.\n\n");

    while(1) {
        char text_to_send[BUFFER_SIZE];

        printf("Teks : ");
        fgets(text_to_send, BUFFER_SIZE, stdin);
        text_to_send[strcspn(text_to_send, "\n")] = 0; 

        if (send(sock, text_to_send, strlen(text_to_send), 0) < 0) {
            perror("Send failed");
            break;
        }

        if (strcmp(text_to_send, "exit") == 0) {
            printf("Mengirim sinyal keluar ke server...\n");
            break;
        }

        memset(buffer, 0, BUFFER_SIZE);
        int bytes_read = read(sock, buffer, BUFFER_SIZE - 1);
        if (bytes_read <= 0) {
             if (bytes_read == 0) {
                printf("\nServer menutup koneksi.\n");
            } else {
                perror("Read failed");
            }
            break;
        }
        buffer[bytes_read] = '\0';

        printf("Server (ROT13) : %s\n\n", buffer);
    }

    close(sock);
    printf("Koneksi ditutup. Sampai jumpa!\n");

    return 0;
}