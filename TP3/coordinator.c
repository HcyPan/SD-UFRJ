#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h> 
#include <string.h>
#include "mythreads.h"

int flag[2];
int turn;
const int MAX = 1e9;
int ans = 0;
int fd[2];

pipe(fd); 

void lock_init()
{
    // Initialize lock by reseting the desire of
    // both the threads to acquire the locks.
    // And, giving turn to one of them.
    flag[0] = flag[1] = 0;
    turn = 0;
}

// Executed before entering critical section
void lock(int self)
{
    // Set flag[self] = 1 saying you want to acquire lock
    flag[self] = 1;
 
    // But, first give the other thread the chance to
    // acquire lock
    turn = 1-self;
 
    // Wait until the other thread looses the desire
    // to acquire lock or it is your turn to get the lock.
    while (flag[1-self]==1 && turn==1-self) ;
}

// Executed after leaving critical section
void unlock(int self)
{
    // You do not desire to acquire lock in future.
    // This will allow the other thread to acquire
    // the lock.
    flag[self] = 0;
}



typedef struct no{ //Definindo estrutura da lista encadeada
	char *valor;
	struct no *prox;
	struct no *ant;
}no;

typedef no lista;


void Inicializar(lista* linicio, lista* lfinal){
	
	//Ajeitando os ponteiros de linicio e lfina

	linicio->prox = lfinal;
	linicio->ant = NULL;

	lfinal->prox = NULL;
	lfinal->ant = linicio;
}

typedef struct thread_data {
    lista* comecoLista;
    lista* fimLista;
} tdata_t;


/*Função Inserir*/
void Inserir(lista* linicio, lista* lfinal, char *string){
	
	//Alocar Espaço na memória para o novo nó que será adicionado
	lista* novo = (lista*)malloc(sizeof(lista));
	
	novo->valor = string;

	novo->prox = lfinal;
	novo->ant = lfinal->ant;

	lfinal->ant->prox = novo;
	lfinal->ant = novo;

}

/*Função Remover*/
void Remover(lista* linicio, lista* lfinal, char *string){

	lista* p; //Ponteiro que irá percorrer a lista.

	p = linicio->prox;

	/*for(p = linicio->prox; p!=lfinal; p = p->prox ){
		
		if((strcmp(string, p->valor))== 0) break;
	}*/
	
	while((p != lfinal) && (strcmp(string, p->valor) != 0) ){
                  p = p->prox;
        }
        

	if( p == lfinal){
		
		
		printf("O elemento %s que você quer remover não está na lista\n", string);  

		}
	else{

		p->ant->prox = p->prox;
		p->prox->ant = p->ant;
		free(p);

	}

}

/*Função Imprimir*/
void Imprimir(lista* linicio, lista* lfinal){
	
	lista* p = linicio->prox; //Ponteiro que vai percorrer a lista a partir do proximo depois da cabeça.

	while( p != lfinal){
		
		printf("Elemento: %s ",p->valor);
		p = p->prox;
	}

printf("\n");

}


//FUNÇÃO PARA RECEBER A CONEXÃO COM OS PROCESSOS
void *comunicaThread(void *vargp)
{
    close(fd[0]);
    int estado = 0;
    //função executada pela thread responsável pelo socket
    //Criando o socket do servidor
    char server_message[10];
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    //Configurando o endereço do servidor
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9004);
    server_address.sin_addr.s_addr = INADDR_ANY;
    //Se conectando à porta configurada
    bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));
    listen(server_socket, 4);
    int envio = 10;
    int client_socket;
    client_socket = accept(server_socket, NULL, NULL);
    while(estado != 4){ 
        recv(client_socket,&server_message, sizeof(server_message), 0);
        //write(fd[1], &server_message, sizeof(server_message));
        printf("Mensagem inserida: %s\n", server_message);
        estado++;
    }
}

void *exclusaoMutua(void *vargp){
    int estado = 0;
    char messageServer[10];
    printf("Algoritmo de exclusão mútua sendo executado\n");
    lista* linicio = (lista*)malloc(sizeof(lista));
	lista* lfinal = (lista*)malloc(sizeof(lista));
    Inicializar(linicio,lfinal);
    close(fd[0]);
    while( estado != 4){
        read(fd[0], &messageServer, sizeof(messageServer));
        Inserir(linicio, lfinal, &messageServer);
        estado++;
    }
    Imprimir(linicio, lfinal);
    close(fd[0]);
}

void *threadInterface(void *vargp){
    printf("Algoritmo de interface sendo executado\n");
}


int main(int argc, char* argv[]){
    pthread_t thread_id[3]; //3 indica a quantidade de identificadores de threads a serem criados;
    srand(time(NULL));

    //Inicializando as duas 'barreiras' da lista duplamente encadeada: o começo e o final.

    pthread_create(&(thread_id[1]), NULL, exclusaoMutua, NULL); //criação da thread
    pthread_create(&(thread_id[2]), NULL, threadInterface, NULL); //criação da thread
    pthread_create(&(thread_id[0]), NULL, comunicaThread, NULL); //criação da thread

    pthread_join(thread_id[0], NULL);
    pthread_join(thread_id[1], NULL); //Aguardar a execução da thread
    pthread_join(thread_id[2], NULL); //Aguardar a execução da thread
    printf("Encerrando");
    return 0;
}
