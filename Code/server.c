#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void rot13(char *str) {
    if (str == NULL) return;
    for (int i = 0; str[i] != '\0'; i++) {
        char c = str[i];
        if (c >= 'a' && c <= 'z') {
            str[i] = (((c - 'a') + 13) % 26) + 'a';
        } else if (c >= 'A' && c <= 'Z') {
            str[i] = (((c - 'A') + 13) % 26) + 'A';
        }
    }
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server interaktif berjalan di port %d...\n", PORT);

    while(1) {
        printf("\n[STATUS] Menunggu koneksi client baru...\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("Accept failed");
            continue; 
        }
        
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &address.sin_addr, client_ip, INET_ADDRSTRLEN);
        printf("[KONEKSI] Diterima dari %s:%d\n", client_ip, ntohs(address.sin_port));

        while(1) {
            char buffer[BUFFER_SIZE] = {0};
            
            int bytes_read = read(new_socket, buffer, BUFFER_SIZE - 1);

            if (bytes_read <= 0) {
                if (bytes_read == 0) {
                    printf("[KONEKSI] Client %s:%d terputus.\n", client_ip, ntohs(address.sin_port));
                } else {
                    perror("Read failed");
                }
                break; 
            }
            
            buffer[bytes_read] = '\0';

            if (strcmp(buffer, "exit") == 0) {
                printf("[SESI] Client mengirim 'exit'. Menutup koneksi.\n");
                break; 
            }

            printf("  [TERIMA] Dari client: %s\n", buffer);

            rot13(buffer);
            printf("  [ENKRIPSI] Hasil ROT13: %s\n", buffer);

            send(new_socket, buffer, strlen(buffer), 0);
            printf("  [KIRIM] Balasan dikirim ke client.\n");
        }

        close(new_socket);
    }

    close(server_fd);
    return 0;
}