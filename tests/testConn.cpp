#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <iostream>

void* connect_to_server(void* arg) {
    const char* server_ip = "127.0.0.1";
    const int server_port = 6667;
	int i = *(int *)arg + 5;
    std::string message = "PASS hi\r\nNICK " + std::to_string(i) + "\r\nUSER a a a :a\r\n";

    // Create a socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error creating socket");
        return NULL;
    }

    // Set up server information
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    if (inet_pton(AF_INET, server_ip, &server_address.sin_addr) <= 0) {
        perror("Invalid server IP address");
        close(client_socket);
        return NULL;
    }

    // Connect to the server
    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("Error connecting to server");
        close(client_socket);
        return NULL;
    }

    // Send a message
    ssize_t bytes_sent = send(client_socket, message.c_str(), message.length(), 0);
    if (bytes_sent == -1)
        perror("Error sending message");
	sleep(60); // ctrl + c here stops the server (related to sending messages on disconnect)
	message = "QUIT :Disconnected\r\n";
    bytes_sent = send(client_socket, message.c_str(), message.length(), 0);
    if (bytes_sent == -1)
        perror("Error sending message");
	sleep(2);
    // Close the socket
    close(client_socket);

    return NULL;
}

int main()
{
    const int num_connections = 1024 - 4;
    pthread_t threads[num_connections];

    for (int i = 0; i < num_connections; ++i) {
		// usleep(1000);
        if (pthread_create(&threads[i], NULL, connect_to_server, &i) != 0) {
            perror("Error creating thread");
            exit(EXIT_FAILURE);
        }
    }
    // Wait for threads to finish
    for (int i = 0; i < num_connections; ++i) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}