#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    char filename[100];

    printf("🔎 Πληκτρολόγησε το όνομα του αρχείου που θες να λάβεις: ");
    scanf("%s", filename);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation error");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("Invalid address\n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        return -1;
    }

    // Στέλνει το όνομα του αρχείου στον server
    send(sock, filename, strlen(filename), 0);

    // Άνοιγμα αρχείου για εγγραφή
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        perror("Δεν μπορώ να δημιουργήσω αρχείο");
        close(sock);
        return -1;
    }

    // Λαμβάνει και γράφει στο αρχείο
    ssize_t bytes_received;
    while ((bytes_received = read(sock, buffer, BUFFER_SIZE)) > 0) {
        fwrite(buffer, 1, bytes_received, fp);
    }

    printf("✅ Το αρχείο αποθηκεύτηκε τοπικά ως '%s'\n", filename);
    fclose(fp);
    close(sock);
    return 0;
}
