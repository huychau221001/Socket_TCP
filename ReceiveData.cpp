#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024

void receiveFile(int listenPort, const char* savePath) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        return;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(listenPort);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Binding failed");
        close(sockfd);
        return;
    }

    if (listen(sockfd, 1) == -1) {
        perror("Listening failed");
        close(sockfd);
        return;
    }

    sockaddr_in clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    int connfd = accept(sockfd, (struct sockaddr*)&clientAddr, &addrLen);
    if (connfd == -1) {
        perror("Acceptance failed");
        close(sockfd);
        return;
    }

    std::ofstream outputFile(savePath, std::ios::binary);
    char buffer[BUFFER_SIZE];
    int bytesRead;

    while ((bytesRead = recv(connfd, buffer, BUFFER_SIZE, 0)) > 0) {
        outputFile.write(buffer, bytesRead);
        std::cout << "Received " << bytesRead << " bytes." << std::endl;
    }

    close(connfd);
    close(sockfd);
    std::cout << "File received successfully and saved as '" << savePath << "'." << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 3 || strcmp(argv[1], "-out") != 0) {
        std::cerr << "Usage: ReceiveData -out <location_store_file> <listen_port>" << std::endl;
        return 1;
    }

    const char* savePath = argv[2];
    int listenPort = std::atoi(argv[3]);

    receiveFile(listenPort, savePath);

    return 0;
}
