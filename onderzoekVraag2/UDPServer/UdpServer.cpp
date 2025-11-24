#include <iostream>
#include <thread>
#include <vector>
#include <cstring>
#include "socket.h"
#include <fstream>
#include <sstream>
#include <chrono>
#include "./ping.h"
#include <algorithm>

#define PORT 1721
#define MTU 1500

int main() {
    
    std::vector<ping> msges;
    __int64 msgCount = 0;
    float average = 0;
    auto averageTripTime = [&msgCount,&average](ping& msg) {
        average += static_cast<float>(msg.GetMillis() / msgCount); 
    };


    std::cout << "UDP Server!\n";
    std::cout << __DATE__ << " " << __TIME__ << std::endl;
#ifdef _WIN32
    WSADATA wsaData;
    (void)WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

    SOCKET serverSock = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSock == INVALID_SOCKET) {
        std::cerr << "Cannot create socket\n";
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed\n";
        closeSocket(serverSock);
        return 1;
    }

    std::cout << "UDP Server listening on port " << PORT << "...\n";


    sockaddr_in clientAddr{};
    socklen_t clientLen = sizeof(clientAddr);

    char buffer[4096];
    while (true) {
        int bytes = recvfrom(serverSock, buffer, sizeof(buffer) - 1, 0,
            (sockaddr*)&clientAddr, &clientLen);
        if (bytes < 0) break;
        buffer[bytes] = '\0';

        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, sizeof(clientIP));
        std::cout << "Received from " << clientIP << ": " << buffer << "\n";
        std::string msg(buffer);
        if (bytes > 0) {
            if (!msg.compare("write") && msges.size() > 0) {
                std::ofstream csvFile("./data.csv", std::ios::app);
                if (!csvFile.is_open()) csvFile.open("./data.csv");
                msgCount = msges.size();
                std::for_each(msges.begin(), msges.end(), averageTripTime);

                csvFile << "\n" << MTU << ',' << average << ',' << static_cast<float>(msges.size() / 100.0f);

                csvFile.close();
                msges.clear();
                average = 0;


                std::string reply = "Server reply: results have been written" ;
                sendto(serverSock, reply.c_str(), (int)reply.size(), 0,
                    (sockaddr*)&clientAddr, clientLen);
            }
            else {
                msges.push_back(msg);
            }
        }

        
    }

    closeSocket(serverSock);

#ifdef _WIN32
    WSACleanup();
#endif

    
    
    return 0;
}

