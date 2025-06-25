# Final Project Sistem Operasi IT

## Peraturan
1. Waktu pengerjaan dimulai hari Kamis (19 Juni 2025) setelah soal dibagikan hingga hari Rabu (25 Juni 2025) pukul 23.59 WIB.
2. Praktikan diharapkan membuat laporan penjelasan dan penyelesaian soal dalam bentuk Readme(github).
3. Format nama repository github “Sisop-FP-2025-IT-[Kelas][Kelompok]” (contoh:Sisop-FP-2025-IT-A01).
4. Setelah pengerjaan selesai, seluruh source code dan semua script bash, awk, dan file yang berisi cron job ditaruh di github masing - masing kelompok, dan link github dikumpulkan pada form yang disediakan. Pastikan github di setting ke publik.
5. Commit terakhir maksimal 10 menit setelah waktu pengerjaan berakhir. Jika melewati maka akan dinilai berdasarkan commit terakhir.
6. Jika tidak ada pengumuman perubahan soal oleh asisten, maka soal dianggap dapat diselesaikan.
7. Jika ditemukan soal yang tidak dapat diselesaikan, harap menuliskannya pada Readme beserta permasalahan yang ditemukan.
8. Praktikan tidak diperbolehkan menanyakan jawaban dari soal yang diberikan kepada asisten maupun praktikan dari kelompok lainnya.
9. Jika ditemukan indikasi kecurangan dalam bentuk apapun di pengerjaan soal final project, maka nilai dianggap 0.
10. Pengerjaan soal final project sesuai dengan modul yang telah diajarkan.

## Kelompok 8

Nama | NRP
--- | ---
Clarissa Aydin Rahmazea | 5027241014
Moch. Rizki Nasrullah | 5027241038
Ahmad Rafi Fadhillah Dwiputra | 5027241068
Maritza Adelia Sucipto | 5027241111

## Deskripsi Soal

> Buatlah sebuah program Encryptor berbasis Remote Procedure Call (RPC) yang terdiri dari dua komponen utama, yaitu client dan server. Dalam program ini, client akan mengirimkan sebuah teks ke server melalui mekanisme RPC. Server kemudian menerima teks tersebut dan melakukan proses enkripsi menggunakan algoritma ROT13 dan setelah proses enkripsi selesai, server akan mengembalikan hasil enkripsi tersebut kepada client sebagai respons.

### Catatan

> Program ini mengimplementasikan sistem Remote Procedure Call (RPC) sederhana menggunakan TCP socket untuk komunikasi antara client dan server. Server melakukan enkripsi teks menggunakan algoritma ROT13 dan mengembalikan hasil enkripsi kepada client.

Struktur repository:
```
Sisop-FP-2025-IT-A08/
├── Code/
│   ├── client.c          # Program client RPC
│   └── server.c          # Program server RPC
├── Papers/
│   ├── 259-Article Text-881-1-10-20121110.pdf
│   └── RPC.pdf
└── README.md
```

## Pengerjaan

### Pengertian Remote Procedure Calls (RPC)

Remote Procedure Call (RPC) adalah sebuah mekanisme komunikasi dalam sistem terdistribusi yang memungkinkan sebuah program untuk memanggil prosedur atau fungsi yang berada di komputer lain seolah-olah dijalankan secara lokal. Konsep ini diperkenalkan secara mendalam oleh Birrell dan Nelson dalam makalah klasik mereka pada tahun 1984 (Implementing Remote Procedure Calls) (Birrell & Nelson, 1984). Dalam sistem RPC, prosedur pemanggil (caller) dan yang dipanggil (callee) berada pada ruang alamat yang berbeda dan dapat dijalankan pada sistem operasi yang berbeda pula, sehingga diperlukan teknik khusus untuk mengatasi kompleksitas komunikasi lintas mesin ini. Salah satu tantangan utamanya adalah bagaimana mengirimkan parameter dan menerima hasil antar sistem yang bisa saja memiliki arsitektur perangkat lunak atau struktur data yang berbeda. Meski demikian, pendekatan RPC terbukti efektif dan telah menjadi fondasi dari banyak sistem terdistribusi modern, karena menyederhanakan komunikasi jaringan melalui abstraksi pemanggilan fungsi jarak jauh.

## Tujuan

Tujuan utama dari proyek RPC yang dikembangkan oleh Birrell dan Nelson adalah untuk menyederhanakan proses komputasi terdistribusi. Mereka mengamati bahwa pembangunan program komunikasi dalam lingkungan penelitian saat itu merupakan tugas yang kompleks, hanya dapat dilakukan oleh sekelompok kecil pakar komunikasi. 

### Tujuan Proyek RPC (Birrell & Nelson, 1984)
1. Menyederhanakan komputasi terdistribusi
   - Membangun sistem terdistribusi dinilai sulit dengan alat yang ada saat itu.
   - RPC diharapkan membuat komunikasi antarmesin semudah pemanggilan prosedur lokal.
     
2. Mendorong adopsi luas oleh pengembang
   - Mempermudah eksperimen dan pengembangan aplikasi terdistribusi, bahkan oleh non-ahli.

3. Menghilangkan hambatan komunikasi yang tidak perlu
   - Menyisakan hanya tantangan fundamental seperti sinkronisasi waktu dan kegagalan komponen.

4. Efisiensi tinggi dalam komunikasi RPC
   - Menghindari latensi yang tinggi agar aplikasi tetap efisien dan tidak menghindari komunikasi.

### 1. Implementasi Server RPC

**Teori**

Server RPC berfungsi sebagai penyedia layanan enkripsi ROT13. Server akan:
- Mendengarkan koneksi pada port 8080
- Menerima teks dari client
- Melakukan enkripsi ROT13
- Mengirimkan hasil enkripsi kembali ke client

ROT13 adalah algoritma enkripsi sederhana yang menggeser setiap huruf alfabetik sebanyak 13 posisi. Untuk huruf a-z: a→n, b→o, ..., n→a, o→b, dst. Begitu juga untuk huruf A-Z.


**Solusi**

Server diimplementasikan dengan fitur-fitur berikut:
```c
// Fungsi enkripsi ROT13
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
```

Server menggunakan TCP socket untuk komunikasi yang reliable, menampilkan status koneksi client, dan dapat melayani multiple client secara berurutan.


### 2. Implementasi Client RPC

**Teori**

Client RPC berfungsi sebagai pengguna layanan enkripsi. Client akan:
- Terhubung ke server pada alamat IP dan port tertentu
- Mengirimkan teks yang ingin dienkripsi
- Menerima hasil enkripsi dari server
- Menampilkan hasil kepada pengguna

**Solusi**

Client diimplementasikan dengan interface interaktif yang memungkinkan pengguna:
- Memasukkan teks secara berulang
- Melihat hasil enkripsi real-time
- Keluar dari program dengan perintah "exit"

```c
// Loop utama client untuk interaksi
while(1) {
    printf("Teks : ");
    fgets(text_to_send, BUFFER_SIZE, stdin);
    
    // Kirim ke server dan terima balasan
    send(sock, text_to_send, strlen(text_to_send), 0);
    read(sock, buffer, BUFFER_SIZE - 1);
    
    printf("Server (ROT13) : %s\n\n", buffer);
}
```
### 3. Cara Menjalankan Program

**Kompilasi:**
```bash
cd Code
gcc -o server server.c
gcc -o client client.c
```
**Menjalankan:**
1. Jalankan server terlebih dahulu:
   ```bash
   ./server
   ```

2. Di terminal lain, jalankan client:
   ```bash
   ./client
   ```

3. Masukkan teks yang ingin dienkripsi, program akan menampilkan hasil ROT13.

**Contoh Penggunaan:**
```
Teks : Hello World
Server (ROT13) : Uryyb Jbeyq

Teks : Sistem Operasi
Server (ROT13) : Fvfgrz Bcrenfv
```


**Video Menjalankan Program**

https://github.com/user-attachments/assets/ee575607-9fc2-46d7-8009-5a4b43264a30

## Daftar Pustaka

1. Stevens, W. Richard. "UNIX Net

work Programming Volume 1: The Sockets Networking API." Addison-Wesley Professional, 2003.
2. RFC 1831 - RPC: Remote Procedure Call Protocol Specification Version 
