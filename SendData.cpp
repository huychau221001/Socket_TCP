#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

const int BUFFER_SIZE = 1024;

void LogSendProgress(const char* fileName, int totalBytesSent) {
    std::ofstream logFile("send_log.txt", std::ios::app);
    if (logFile) {
        logFile << "Sent " << totalBytesSent << " bytes of file " << fileName << std::endl;
        logFile.close();
    }
}

bool SendData(const char* destAddress, const char* data, int dataSize) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cerr << "Failed to create socket." << std::endl;
        return false;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080); // Port của máy nhận
    if (inet_pton(AF_INET, destAddress, &serverAddr.sin_addr) <= 0) {
        std::cerr << "Invalid address." << std::endl;
        close(sockfd);
        return false;
    }

    if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Failed to connect to the server." << std::endl;
        close(sockfd);
        return false;
    }

    int bytesSent = 0;
    while (bytesSent < dataSize) {
        int bytes = send(sockfd, data + bytesSent, dataSize - bytesSent, 0);
        if (bytes == -1) {
            std::cerr << "Failed to send data." << std::endl;
            close(sockfd);
            return false;
        }
        bytesSent += bytes;
    }
    if (SendData(destAddress, fileName.c_str(), fileName.length()) == false) {
        std::cerr << "Failed to send file name." << std::endl;
        return false;
    }

    char buffer[bufferSize];
    int bytesRead;
    int totalBytesSent = 0;

    while ((bytesRead = file.readsome(buffer, sizeof(buffer))) > 0) {
        if (!SendData(destAddress, buffer, bytesRead)) {
            std::cerr << "Failed to send file." << std::endl;
            file.close();
            return false;
        }

        totalBytesSent += bytesRead;
        LogSendProgress(fileName.c_str(), totalBytesSent); // Ghi lại tiến trình gửi
        std::cout << "Sent " << totalBytesSent << " bytes." << std::endl;
    }

    file.close();
    std::cout << "File sent successfully." << std::endl;
    LogSendProgress(fileName.c_str(), totalBytesSent); // Ghi lại hoàn thành việc gửiceive.cpp
    close(sockfd);
    return true;

    
}

void SendText(const char* destAddress, const char* text) {
    int textSize = strlen(text);
    if (SendData(destAddress, text, textSize)) {
        std::cout << "Text sent successfully." << std::endl;
    } else {
        std::cerr << "Failed to send text." << std::endl;
    }
}

void SendFile(const char* destAddress, const char* filePath, int bufferSize) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file." << std::endl;
        return;
    }

    std::string fileName = std::string(filePath);
    size_t slashPos = fileName.find_last_of("/");
    if (slashPos != std::string::npos) {
        fileName = fileName.substr(slashPos + 1);
    }

    if (SendData(destAddress, fileName.c_str(), fileName.length()) == false) {
        std::cerr << "Failed to send file name." << std::endl;
        return;
    }

    char buffer[bufferSize];
    int bytesRead;
    int totalBytesSent = 0;

    while ((bytesRead = file.readsome(buffer, sizeof(buffer))) > 0) {
        if (!SendData(destAddress, buffer, bytesRead)) {
            std::cerr << "Failed to send file." << std::endl;
            file.close();
            return;
        }

        totalBytesSent += bytesRead;
        std::cout << "Sent " << totalBytesSent << " bytes." << std::endl;
    }

    file.close();
    std::cout << "File sent successfully." << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 4 || (strcmp(argv[1], "SendData") != 0)) {
        std::cerr << "Usage: SendData <destination_address> <command> <...>" << std::endl;
        return 1;
    }

    const char* destAddress = argv[2];
    const char* command = argv[3];

    if (strcmp(command, "SendText") == 0 && argc >= 5) {
        const char* text = argv[4];
        SendText(destAddress, text);
    } else if (strcmp(command, "SendFile") == 0 && argc >= 7) {
        const char* filePath = argv[4];
        int bufferSize = atoi(argv[5]);
        SendFile(destAddress, filePath, bufferSize);
    } else {
        std::cerr << "Invalid command." << std::endl;
        return 1;
    }

    return 0;
}
