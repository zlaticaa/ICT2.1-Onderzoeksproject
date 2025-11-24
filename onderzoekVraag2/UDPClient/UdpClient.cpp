#include <iostream>
#include <string>
#include "socket.h"
#include <fstream>
#include <chrono>
#include <sstream>

#define PORT 1721
#define IP_ADDRESS "127.0.0.1"  //localhost loopback address
#define MTU 1500 //the current MTU of this test run
int main() {
    std::cout << "UDP CLient!\n";
    std::cout << __DATE__ << " " << __TIME__ << std::endl; // log date and time of compilation, not runtime
#ifdef _WIN32
    WSADATA wsaData;
    (void)WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

    SOCKET clientSock = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSock == INVALID_SOCKET) {
        std::cerr << "Cannot create socket\n";
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, IP_ADDRESS, &serverAddr.sin_addr);

    while (true) {
        


        for (int i= 0; i < 100; i++) {
            auto now = std::chrono::system_clock::now();
            auto duration = now.time_since_epoch();
            auto millisNow = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
            std::string msgi;
            std::stringstream ss(msgi);
            ss << millisNow.count() << " ";
            //std::string msgi = time + " " + std::string(4096 - time.size() - 1, 'A');
            sendto(clientSock, msgi.c_str(), (int)msgi.size(), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
        }

        std::string msg = "write";
        sendto(clientSock, msg.c_str(), (int)msg.size(), 0,
            (sockaddr*)&serverAddr, sizeof(serverAddr));
        std::string out;
        std::cout << "repeat test (or 'quit'): ";
        std::getline(std::cin, out);
        if (msg == "quit") break;

        char buffer[4096];
        sockaddr_in fromAddr{};
        socklen_t fromLen = sizeof(fromAddr);
        
    }

    closeSocket(clientSock);

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}
