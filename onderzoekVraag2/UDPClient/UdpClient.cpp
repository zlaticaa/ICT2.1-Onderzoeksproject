#include <iostream>
#include <string>
#include "socket.h"
#include <fstream>
#include <chrono>
#include <sstream>

#define PORT 1721
#define IP_ADDRESS "192.168.137.255"  //broadcast to port 1721 on your local area network (change this to the ip of your personal hotspot)
//#define MTU 1500 //the current MTU of this test run
#define AmountOfPings 1000

/*
    you have to change the mtu-value manually by running a command prompt as administrator and typing
    netsh interface ipv4 set subinterface "Wi-Fi" mtu=[MTU value] store=persistent
    for this test to work as intended you must use the same MTU as the server

    the amount of pings and the mtu value need to be the same in both tests or the results might not be accurate
*/

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
        
        //sends an amount of pings equal AmountOfPings (in dit geval 1000)

        for (int i= 0; i <AmountOfPings; i++) {
            //haalt de huidige tijd op
            auto now = std::chrono::system_clock::now();
            auto duration = now.time_since_epoch();
            auto microsNow = std::chrono::duration_cast<std::chrono::microseconds>(duration);

            //maakt een stringstream om de msg te contrueren
            std::stringstream msgPing;
            msgPing << microsNow.count() << " " << std::string(1000, 'A') << std::string(1000, 'B') << std::string(1000, 'C') << std::string(1000, 'D');
            //verstuurt het bericht
            sendto(clientSock, msgPing.str().c_str(), msgPing.str().size(), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
            Sleep(3);
        }
        while (true) {
            //stuurt een keer write en herhaalt als die niet aankomt bij de server
            std::string msg = "write";
            sendto(clientSock, msg.c_str(), (int)msg.size(), 0,
                (sockaddr*)&serverAddr, sizeof(serverAddr));

            std::cout << "succesful write [y/n]: ";

            std::string reply;
            std::getline(std::cin, reply);
            if (!reply.compare("y")) break;
        }
        //herhaal de test behalve als de user stopt
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
