#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <pthread.h> 
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h> 

int portCoord1 = 9004, portCoord2 = 9005, portCoord3 = 9006;

//Thread 1 - Coordenador que recebe concexão dos processos
void *coord1(void *vargp){
    int server_message = 0;
    //Criando o socket do servidor
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    //Configurando o endereço do servidor
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(portCoord1);
    server_address.sin_addr.s_addr = INADDR_ANY;

    //Se conectando à porta configurada
    bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));
    
    listen(server_socket, 2);
    
    int client_socket;
    client_socket = accept(server_socket, NULL, NULL);
    // for (int i = 0; i < qntdNum; i++){
    //     send(client_socket, &randomNum, sizeof(i), 0);
    //     randomNum = somaN(randomNum, delta);
    //     recv(client_socket,&server_message, sizeof(server_message), 0);
    //     if (server_message != 0){
    //         printf("É primo: %d\n",server_message);
    //     }
    //     if (i + 1 == qntdNum){
    //         int finalSocket = 0;
    //         send(client_socket, &finalSocket, sizeof(i), 0);
    //     }
    // }        
    printf("Encerrando");
    return 0;
} 

//Thread 2 - Coordenador que executa a exclusão mutua
void *coord2(void *vargp){
    int server_message = 0;
    //Criando o socket do servidor
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    //Configurando o endereço do servidor
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(portCoord2);
    server_address.sin_addr.s_addr = INADDR_ANY;

    //Se conectando à porta configurada
    bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));
    
    listen(server_socket, 2);
    
    int client_socket;
    client_socket = accept(server_socket, NULL, NULL);
    // for (int i = 0; i < qntdNum; i++){
    //     send(client_socket, &randomNum, sizeof(i), 0);
    //     randomNum = somaN(randomNum, delta);
    //     recv(client_socket,&server_message, sizeof(server_message), 0);
    //     if (server_message != 0){
    //         printf("É primo: %d\n",server_message);
    //     }
    //     if (i + 1 == qntdNum){
    //         int finalSocket = 0;
    //         send(client_socket, &finalSocket, sizeof(i), 0);
    //     }
    // }        
    printf("Encerrando");
    return 0;
} 

//Thread 3 - Coordenador que atende a interface
void *coord3(void *vargp){
    int server_message = 0;
    //Criando o socket do servidor
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    //Configurando o endereço do servidor
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(portCoord3);
    server_address.sin_addr.s_addr = INADDR_ANY;

    //Se conectando à porta configurada
    bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));
    
    listen(server_socket, 2);
    
    int client_socket;
    client_socket = accept(server_socket, NULL, NULL);
    // for (int i = 0; i < qntdNum; i++){
    //     send(client_socket, &randomNum, sizeof(i), 0);
    //     randomNum = somaN(randomNum, delta);
    //     recv(client_socket,&server_message, sizeof(server_message), 0);
    //     if (server_message != 0){
    //         printf("É primo: %d\n",server_message);
    //     }
    //     if (i + 1 == qntdNum){
    //         int finalSocket = 0;
    //         send(client_socket, &finalSocket, sizeof(i), 0);
    //     }
    // }        
    printf("Encerrando");
    return 0;
} 

int main(){
    int K = 3; 
    srand(time(0)); //hora atual como semente do numero aleatório
    pthread_t thread_id[K]; //cria identificador do tipo pthread_t
    printf("Before Thread\n");
    pthread_create(&(thread_id[0]), NULL, coord1, NULL); //criação da thread
    pthread_create(&(thread_id[1]), NULL, coord2, NULL); //criação da thread
    pthread_create(&(thread_id[2]), NULL, coord3, NULL); //criação da thread
    pthread_join(thread_id[0], NULL); //aqui a thread espera a filha
    pthread_join(thread_id[1], NULL); //aqui a thread espera a filha
    pthread_join(thread_id[2], NULL); //aqui a thread espera a filha
    printf("After Thread\n");
}