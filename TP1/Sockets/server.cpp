#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <sstream>
#define PORT 8080

// convert int to string
std::string int2str(int x){
    std::stringstream ss;
    ss << x;
    return ss.str();
}


int main()
{
    int server_fd, new_socket, valread;
    sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char connMsg[1024], sendMsg[20], resMsg[20], strZero[] = "0";
    char hello[] = "Hello from server";

    // create TCP protocol socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // bind socket to port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Socket started at port: " << PORT << "\n";

    // listen for connections in the socket
    if (listen(server_fd, 3) == -1)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    //accept connections
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // print connection string
    valread = recv(new_socket, connMsg, 1024, 0);
    printf("%s\n", connMsg);

    int userNum, init = 1, fin, mid;

    std::cout << "Insert how many numbers to send: ";
    std::cin >> userNum;
    std::cout << "OK! " << userNum << " random numbers will be sent \n";

    // send numbers
    for(int i = 0; i <= userNum; i++){
        // delta changes for every iteration
        int dt = rand() % 100;
        // in this case we dont send the 1 first
        if(i == 0){
            fin = init + dt;
        } else {
            fin = mid + dt;
        }
        mid = fin;
        std::string res = int2str(fin);

        if(i != userNum){
            res.copy(sendMsg, res.size());
            std::cout << "Number generated: " << fin << "\n";
            send(new_socket, sendMsg, 20, 0);
        }else {
            // last number is 0
            std::cout << "Program finished";
            send(new_socket, strZero, 20, 0);
            exit(0);
        }
        valread = recv(new_socket, resMsg, 20, 0);
        // receive prime flag and print result
        if(std::strcmp(resMsg,"true") == 0){
            std::cout << "YEAH!! " << fin << " is Prime! \n";
        } else {
            std::cout << "OH NO! " << fin << " is not Prime! \n";
        }
    }
    return 0;
}