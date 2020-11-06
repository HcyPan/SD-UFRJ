#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <pthread.h> 
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h> 

//Parametros para teste
int N = 2, R = 10, K = 1;

//Função executada por cada thread
void *process(void *vargp){
    int *thread_num = (int*)vargp;
    char thread_str = *thread_num + '0';
    for(int i = 0; i<R; i++){
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
        char mensagem[10] = {"1", "|", thread_str, "|"};
        int len = sizeof(mensagem);
        for(int i = len; i <10; i++){
            if(i = 9){
                mensagem[i] = "\0";
            }else{
                mensagem[i] = "0";
            }
        }
        send(network_socket, &mensagem, sizeof(mensagem), 0);
        // while(estado == 1){
        //     recv(network_socket, &server_response, sizeof(server_response), 0);
        //     if(testePrimo(server_response) == 1 && server_response != 0){
        //         printf("Enviando primo como resposta: %d\n", server_response);
        //         send(network_socket, &server_response, sizeof(server_response), 0);
        //     } else if (server_response == 0){
        //         estado = 0;
        //     } else{
        //         int naoPrimo = 0;
        //         send(network_socket, &naoPrimo, sizeof(naoPrimo), 0);
        //     }
            
        // }
        close(network_socket);
    }
}

int main(){
    pthread_t thread_id[N]; //cria identificador do tipo pthread_t
    printf("Before Thread\n");
    for(int j = 0; j < N; j++){
        pthread_create(&(thread_id[j]), NULL, process, &j); //criação da thread
    }
    for(int j = 0; j < 5; j++){ 
        pthread_join(thread_id[j], NULL); //aqui a thread espera a filha
    }
    printf("After Thread\n");
}