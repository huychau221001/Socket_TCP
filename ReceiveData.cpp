#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

const int BUFFER_SIZE = 1024;

void LogReceiveProgress(const char* fileName, int totalBytesReceived) {
    std::ofstream logFile("receive_log.txt", std::ios::app);
    if (logFile) {
        logFile << "Received " << totalBytesReceived << " bytes of file " << fileName << std::endl;
        logFile.close();
    }
}

bool ReceiveData(int sockfd, const char* outputDirectory) {
    char buffer[BUFFER_SIZE];
    int totalBytesReceived = 0;
    int bytesRead;

    std::string outputFile;
    std::ofstream outFile;

    while ((bytesRead = recv(sockfd, buffer, sizeof(buffer), 0)) > 0) {
        if (totalBytesReceived == 0) {
            // Nhận tên tập tin từ máy gửi (nếu là tập tin)
            buffer[bytesRead] = '\0';
            outputFile = std::string(outputDirectory) + "/" + buffer;

            // Mở tập tin để ghi
            outFile.open(outputFile, std::ios::binary);
            if (!outFile) {
                std::cerr << "Failed to create output file." << std::endl;
                return false;
            }
        } else {
            // Ghi dữ liệu vào tập tin
            outFile.write(buffer, bytesRead);
        }

        totalBytesReceived += bytesRead;
        std::cout << "Received " << totalBytesReceived << " bytes." << std::endl;
        LogReceiveProgress(outputFile.c_str(), totalBytesReceived);
    }

    if (bytesRead < 0) {
        std::cerr << "Error while receiving data." << std::endl;
        return false;
    }

    outFile.close();
    
    std::cout << "Data received successfully and saved to: " << outputFile << std::endl;
    LogReceiveProgress(outputFile.c_str(), totalBytesReceived);
    return true;
}

int main(int argc, char* argv[]) {
    if (argc != 4 || (strcmp(argv[1], "ReceiveData") != 0) || (strcmp(argv[2], "-out") != 0)) {
        std::cerr << "Usage: ReceiveData -out <output_directory>" << std::endl;
        return 1;
    }

    const char* outputDirectory = argv[3];

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080); // Port của máy nhận
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Binding error." << std::endl;
        close(sockfd);
        return 1;
    }

    if (listen(sockfd, 5) == -1) {
        std::cerr << "Listening error." << std::endl;
        close(sockfd);
        return 1;
    }

    std::cout << "Waiting for incoming connections..." << std::endl;

    while (true) {
        sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        int clientSockfd = accept(sockfd, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSockfd == -1) {
            std::cerr << "Error accepting connection." << std::endl;
            close(clientSockfd);
            continue;
        }

        std::cout << "Accepted connection from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;

        if (!ReceiveData(clientSockfd, outputDirectory)) {
            std::cerr << "Error receiving data from client." << std::endl;
        }

        close(clientSockfd);
    }

    close(sockfd);
    return 0;
}
