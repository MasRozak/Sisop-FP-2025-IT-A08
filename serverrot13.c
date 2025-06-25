#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Fungsi untuk mengenkripsi teks dengan ROT13 (tidak berubah)
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

    // 1. Membuat socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 2. Binding
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // 3. Listening
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server interaktif berjalan di port %d...\n", PORT);

    // Loop utama: menunggu koneksi baru
    while(1) {
        printf("\n[STATUS] Menunggu koneksi client baru...\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("Accept failed");
            continue; // Jangan matikan server, tunggu client berikutnya
        }
        
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &address.sin_addr, client_ip, INET_ADDRSTRLEN);
        printf("[KONEKSI] Diterima dari %s:%d\n", client_ip, ntohs(address.sin_port));

        // Loop sesi: melayani satu client yang terhubung
        while(1) {
            char buffer[BUFFER_SIZE] = {0};
            
            // Menerima data dari client
            int bytes_read = read(new_socket, buffer, BUFFER_SIZE - 1);

            // Cek jika client menutup koneksi atau terjadi error
            if (bytes_read <= 0) {
                if (bytes_read == 0) {
                    printf("[KONEKSI] Client %s:%d terputus.\n", client_ip, ntohs(address.sin_port));
                } else {
                    perror("Read failed");
                }
                break; // Keluar dari loop sesi, siap menerima client baru
            }
            
            buffer[bytes_read] = '\0';

            // Cek jika client mengirim sinyal "exit"
            if (strcmp(buffer, "exit") == 0) {
                printf("[SESI] Client mengirim 'exit'. Menutup koneksi.\n");
                break; // Keluar dari loop sesi
            }

            printf("  [TERIMA] Dari client: %s\n", buffer);

            // "RPC": Panggil fungsi enkripsi
            rot13(buffer);
            printf("  [ENKRIPSI] Hasil ROT13: %s\n", buffer);

            // Mengirim balasan terenkripsi ke client
            send(new_socket, buffer, strlen(buffer), 0);
            printf("  [KIRIM] Balasan dikirim ke client.\n");
        }

        // Tutup socket untuk client ini setelah sesinya selesai
        close(new_socket);
    }

    close(server_fd);
    return 0;
}