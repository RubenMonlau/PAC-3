#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 9000
#define BUFFER_SIZE 1024

void startClient() {
    int client_fd;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE] = {0};

    // Crear socket
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    // Convertir l'adreça IP
    if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0) {
        perror("Adreça invàlida o no suportada");
        exit(EXIT_FAILURE);
    }

    // Connectar al servidor
    if (connect(client_fd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("No es pot connectar al servei");
        exit(EXIT_FAILURE);
    }

    std::cout << "Connexió establerta amb el servei." << std::endl;

    // Rebre data i hora
    read(client_fd, buffer, BUFFER_SIZE);
    std::cout << "Hora i data rebudes: " << buffer << std::endl;

    close(client_fd); // Tancar la connexió
}

int main() {
    startClient();
    return 0;
}