#include <iostream>
#include <ctime>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // Incloure inet_ntop
#include <unistd.h>

#define PORT 9000
#define BUFFER_SIZE 1024

void startServer() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    char client_ip[INET_ADDRSTRLEN]; // Buffer per a l'adreça IP del client

    // Crear socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }

    // Configuració de l'adreça
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Lligar el socket al port
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Error al lligar el socket");
        exit(EXIT_FAILURE);
    }

    // Escoltar connexions
    if (listen(server_fd, 3) < 0) {
        perror("Error a l'escoltar");
        exit(EXIT_FAILURE);
    }

    std::cout << "Servei iniciat. Escoltant al port " << PORT << "..." << std::endl;

    while (true) {
        // Acceptar connexió
        if ((client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Error al acceptar connexió");
            exit(EXIT_FAILURE);
        }

        // Convertir l'adreça IP del client a format llegible
        inet_ntop(AF_INET, &address.sin_addr, client_ip, INET_ADDRSTRLEN);

        std::cout << "Connexió acceptada des del client " << client_ip << std::endl;

        // Obtenir l'hora i la data actuals
        std::time_t now = std::time(nullptr);
        std::string datetime = std::ctime(&now);

        // Enviar la data i hora al client
        send(client_socket, datetime.c_str(), datetime.size(), 0);
        std::cout << "Hora i data enviades: " << datetime << std::endl;

        close(client_socket); // Tancar la connexió amb el client
    }
}

int main() {
    startServer();
    return 0;
}