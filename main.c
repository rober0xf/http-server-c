#include <arpa/inet.h>
#include <bits/sockaddr.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BACKLOG 20 // how many connections will be wait before requests are refused

void handle_error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main() {
    // file descriptor. endpoint for comunication
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1) {
        handle_error("error socket");
    }

    struct sockaddr_in my_addr;
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(8080);                     // htons makes sure that the number is stored in memory in network byte order
    inet_pton(AF_INET, "127.0.0.1", &my_addr.sin_addr); // inet_pton converts ipv4 and ipv6 addresses from text to binary form

    int result = bind(sfd, (struct sockaddr *)&my_addr, sizeof(my_addr)); // the sizeof will cast to socklent_t
    if (result == -1) {
        handle_error("error bind");
    }

    // marks the socket as a passive socket that will be used to accept incoming connection requests
    result = listen(sfd, BACKLOG);
    if (result == -1) {
        handle_error("error listen");
    }

    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr); // accept as a third argument takes a pointer to an socklen_t
    int cfd = accept(sfd, (struct sockaddr *)&client_addr, &client_addr_size);
    if (cfd == -1) {
        handle_error("error accept");
    }

    char message[] = "message to the client\n";
    result = write(cfd, message, strlen(message)); // we could use sizeof(msg)-1, but it cant handle pointers. this isnt the case but anyways
    if (result == -1) {
        handle_error("error write");
    }

    char buffer[1024] = {0};
    result = read(cfd, buffer, sizeof(buffer));
    printf("%s\n", buffer);
    if (result == -1) {
        handle_error("error read");
    }

    // closes a file descriptor, so that it no longer refers to any file and may be reused
    result = close(sfd);
    if (result == -1) {
        handle_error("error close");
    }

    printf("it works\n");
    return 0;
}
