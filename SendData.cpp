#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024

void sendText(const char* destAddr, const char* text, int destPort) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        return;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(destPort);
    if (inet_pton(AF_INET, destAddr, &serverAddr.sin_addr) <= 0) {
        perror("Invalid address");
        close(sockfd);
        return;
    }

    if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Connection failed");
        close(sockfd);
        return;
    }

    send(sockfd, text, strlen(text), 0);
    close(sockfd);
    std::cout << "Text sent successfully." << std::endl;
}

void sendFile(const char* destAddr, const char* filePath, int destPort, int bufferSize) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file." << std::endl;
        return;
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        return;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(destPort);
    if (inet_pton(AF_INET, destAddr, &serverAddr.sin_addr) <= 0) {
        perror("Invalid address");
        close(sockfd);
        return;
    }

    if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Connection failed");
        close(sockfd);
        return;
    }

    char buffer[BUFFER_SIZE];
    while (!file.eof()) {
        file.read(buffer, bufferSize);
        send(sockfd, buffer, file.gcount(), 0);
    }

    file.close();
    close(sockfd);
    std::cout << "File sent successfully." << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: SendData <destination_address> <destination_port> <options>" << std::endl;
        return 1;
    }

    const char* destAddr = argv[1];
    int destPort = std::atoi(argv[2]);

    if (strcmp(argv[3], "SendText") == 0 && argc >= 5) {
        sendText(destAddr, argv[4], destPort);
    } else if (strcmp(argv[3], "SendFile") == 0 && argc >= 6) {
        const char* filePath = argv[4];
        int bufferSize = std::atoi(argv[5]);
        sendFile(destAddr, filePath, destPort, bufferSize);
    } else {
        std::cerr << "Invalid command or insufficient arguments." << std::endl;
        return 1;
    }

    return 0;
}
