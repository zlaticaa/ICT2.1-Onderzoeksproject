#include <iostream>
#include <string>
#include "socket.h"
#include <fstream>
#include <chrono>
#include <sstream>

#define PORT 1721
#define IP_ADDRESS "127.0.0.1"  //localhost loopback address
#define MTU 1500 //the current MTU of this test run
#define AmountOfPings 1000
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
        


        for (int i= 0; i <AmountOfPings; i++) {
            auto now = std::chrono::system_clock::now();
            auto duration = now.time_since_epoch();
            auto millisNow = std::chrono::duration_cast<std::chrono::milliseconds>(duration);

            std::stringstream msgPing;
            msgPing << millisNow.count() << " " << std::string(1000, 'A') << std::string(1000, 'B') << std::string(1000, 'C') << std::string(1000, 'D');
            
            sendto(clientSock, msgPing.str().c_str(), msgPing.str().size(), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
            Sleep(3);
        }
        while (true) {
            std::string msg = "write";
            sendto(clientSock, msg.c_str(), (int)msg.size(), 0,
                (sockaddr*)&serverAddr, sizeof(serverAddr));

            std::cout << "succesful write [y/n]: ";

            std::string reply;
            std::getline(std::cin, reply);
            if (!reply.compare("y")) break;
        }
        std::string out;
        std::cout << "repeat test (or 'quit'): ";
        std::getline(std::cin, out);
        if (out == "quit") break;
        
    }

    closeSocket(clientSock);

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}
