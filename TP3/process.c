#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h> 

#define N 4 //Quantidade de processos

int main(int argc, char* argv[]){
    //criando socket
    char BUFFER[10];
    int network_socket, estado = 1; 
    network_socket = socket(AF_INET, SOCK_STREAM, 0);

    //Indicação do endereço para o socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = ntohs(9004);
    server_address.sin_addr.s_addr = INADDR_ANY;

    //TRATAMENTO DE ERROS DURANTE A CONEXÃO COM O SOCKET
    int connection_status = connect(network_socket, (struct sockaddr*) &server_address, sizeof(server_address));
    if (connection_status == -1){
        printf("Um erro ocorreu durante a conexão com socket.\n\n");
        return 1;
    }

    //Receber dados do servidor 
    int server_response;
    int id = fork();
    for(int i=0; i < (N-2); i++)
    {
        if (id != 0){ //Processo pai
            id = fork();
        }
    }
    int comando = 1;
    int pidPro = getpid();
    int complemento = sprintf(BUFFER,"%d|%d|",comando, pidPro);
    for (int i = complemento; i < 11; i++){
        BUFFER[i] = '0';
    }
    send(network_socket, &BUFFER, sizeof(BUFFER), 0);
    printf("%s\n", BUFFER);
    close(network_socket);
    return 0;
}
