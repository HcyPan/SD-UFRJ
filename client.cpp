#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <iostream>
#define PORT 8080 

// check if is prime function
bool isPrime(int num){
    bool flag=true;
    for(int i = 2; i <= num / 2; i++) {
       if(num % i == 0) {
          flag = false;
          break;
       }
    }
    return flag;
}
   
int main() 
{ 
    int sock = 0, valread; 
    sockaddr_in serv_addr; 
    char hello[] = "Connection requested \n"; 
    char prime[] = "true";
    char notPrime[] = "false";
    char buffer[20]; 

    // create TCP protocol socket(SOCK_STREAM)
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
    
    // connect to existing socket
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 
    // send connection string
    send(sock , hello , strlen(hello) , 0 ); 

    // check if number is prime, return flag
    while(1){
        valread = recv( sock , buffer, 20, 0); 
        int num = atoi(buffer);
        std::cout << num << " received! \n";
        if(isPrime(num)){
            send(sock, prime, 20, 0);
        } else{
            send(sock, notPrime, 20, 0);
        }
        std::cout << "Response semt! \n";
        if(num == 0){
            std::cout << "Program finished";
            exit(0);
        }
    }
    return 0; 
} 