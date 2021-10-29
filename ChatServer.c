/*
CODIGO FEITO PARA A CADEIRA DE SISTEMAS DISTRIBUIDOS - 2020.2 PROF. ERICA GALLINDO

DUPLA:	FRED DANIEL VARELOD A SILVA
		LUAN COSME DOS SANTOS

CODIGO MODELO: https://gist.github.com/Abhey/47e09377a527acfc2480dbc5515df872
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

int clientCount = 0;

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

/*struct sockaddr_in serverAddr;
memset(&serverAddr, '\0', sizeof(serverAddr));
serverAddr.sin_family = AF_INET;
serverAddr.sin_port = htons(PORT);
serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");*/

struct client{

	int index;
	int sockID;
	struct sockaddr_in clientAddr;
	int len;
	char *cliente_ip;
	int cliente_port;
	//char nome[20];

};
char tempo(){ // PARA PRINTAR O HORARIO NA TELA.
    time_t rawtime;
    struct tm * timeinfo;
    time( &rawtime );
    timeinfo = localtime( &rawtime );
    return printf("[%02d/%02d/%d %02d:%02d:%02d]", timeinfo->tm_mday, timeinfo->tm_mon+1, timeinfo->tm_year+1900, timeinfo->tm_hour, timeinfo->tm_min,timeinfo->tm_sec);
}

struct client Client[1024];
pthread_t thread[1024];

void * doNetworking(void * ClientDetail){

	struct client* clientDetail = (struct client*) ClientDetail;
	int index = clientDetail -> index;
	int clientSocket = clientDetail -> sockID;
	//char ip = clientDetail -> ip;
	//int port = ClientDetail -> port;

	
	
	printf("Conexão aceita de ID %d em [%s:%d]\n",index+1,Client[index].cliente_ip,Client[index].cliente_port);

	/*char seu_ID[15];
	strcpy(seu_ID,"Seu ID é: ");
	int ult,numero = index+1;
	ult = strlen(seu_ID);
	seu_ID[ult] = numero+'0';
	send(clientSocket,seu_ID,1024,0);*/
	char str[20];
	sprintf(str, "Seu ID é: %d", index+1);
	send(clientSocket,str,1024,0);

	
	while(1){

		char data[1024], on_off[1024], inexistente[30] = "[!][CONDIÇÃO NÃO EXISTE!]";
		int read = recv(clientSocket,data,1024,0); // RECEBE A CONDÇÃO, SE VAI SER ENVIAR, SAIR OU LISTAR
		data[read] = '\0';

		char output[1024];

		if(strcmp(data,"listar") == 0){

			int l = 0;

			for(int i = 0 ; i < clientCount ; i ++){

				if(i != index && Client[i].sockID != 0)
					l += snprintf(output + l,1024,"ID %d %s %d.\n",i + 1,Client[i].cliente_ip,Client[i].cliente_port);

			}

			send(clientSocket,output,1024,0);
			continue;

		}else if(strcmp(data,"enviar") == 0){ // CLIENTE QUER ENVIAR MENSAGEM

			read = recv(clientSocket,data,1024,0); // RECEBE PARA QUEM O CLIENTE QUER ENVIAR A MENSAGEM
			data[read] = '\0';
			char Idd[50];
			int id = atoi(data) - 1; // PEGA O ID QUE ESTA EM CHAR E TRANSFERE PARA INT, E DIMINUI 1
										//PARA QUE FIQUE NO PADRÃO DO SERVIDOR 0 ~ n

			if(Client[id].sockID != 0 || id == -1){ // SE O SOCKET NAO FOR 0 (OU SEJA ELE ESTA ONLINE)
													// E COMO A GENTE COLOCOU A MENSAGEM GLOBAL COMO 0, QUANDO ELE
													// TRANSFERE PARA INT ELE FICA -1, ENTÃO TAMBÉM ACEITAMOS ID -1
				if(id >= 0){ // OU SEJA, CLIENTE ONLINE
					
					read = recv(clientSocket,data,1024,0); // MENSAGEM
					data[read] = '\0';
					printf("\nMsg do ID %d [%s:%d]: para ID %d [%s:%d]",index+1,Client[index].cliente_ip,Client[index].cliente_port,id+1,Client[id].cliente_ip,Client[id].cliente_port);
					tempo();
					printf(" %s\n",data);
					
					sprintf(Idd, " [Enviada do ID %d]", index+1);
					strcat(data, Idd);

					send(Client[id].sockID,data,1024,0); // ENVIA PARA O CLIENTE(RECEIVER) EM QUESTÃO A MENSAGEM

					strcpy(on_off,":1:");
					send(Client[index].sockID,on_off,1024,0); // ENVIA PARA O CLIENTE(SENDER) QUE A MENSAGEM FOI ENVIADA COM SUCESSO

				}else{ // MENSAGEM GLOBAL(PARA TODOS OS USUARIOS)
					read = recv(clientSocket,data,1024,0);
					data[read] = '\0';
					printf("\nMsg do ID %d em enviada para todos em ",index+1); // PRINTA PRO CLIENTE QUE O CLIENTE ENVIOU A MENSAGEM GLOBAL
					tempo();
					printf(": %s\n",data);
					//char str[10] = "Curso";
					sprintf(Idd, " [MENSAGEM ENVIADA PARA TODOS DO ID %d]",index+1);
  					strcat(data, Idd);
					for(int i = 0 ; i < clientCount ; i ++){
						if(i != index && Client[i].sockID != 0)
							send(Client[i].sockID,data,1024,0);
					}

					
					//send(Client[id].sockID,data,1024,0);

				}
			}else{
				/*strcpy(desligado, ":0:");
				send(Client[index].sockID,desligado,10,0);*/
				strcpy(on_off, ":0:");
				send(Client[index].sockID,on_off,1024,0);
			}

		}else if(strcmp(data,"sair") == 0){ //QUANDO O USUARIO SAIR O SERVER VAI PEGAR O SEU SOCKET E SETAR COMO 0
										//POIS QUANDO SOCKET É '0' O CLIENTE NAO ESTA ONLINE OU NAO EXISTE

			printf("\nCliente do ID %d no socket %d em [%s:%d] saiu!\n",index+1,Client[index].sockID,Client[index].cliente_ip,Client[index].cliente_port);		
			Client[index].sockID = 0;	

		}else{
			//strcpy(data,inexistente);
			//printf("%s",inexistente);
			send(Client[index].sockID,inexistente,1024,0);
		}

	}

	return NULL;

}

int main(){

	int serverSocket = socket(PF_INET, SOCK_STREAM, 0);

	struct sockaddr_in serverAddr;

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(8080);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	int porta = ntohs(serverAddr.sin_port);

	if(bind(serverSocket,(struct sockaddr *) &serverAddr , sizeof(serverAddr)) == -1) return 0;

	if(listen(serverSocket,1024) == -1) return 0;

	printf("[\t\t. . . Server iniciou . . .\t\t]\n\n");
	printf("[?] Para sair: ctrl + c\n\n");

	while(1){
		//QUANDO FOR ACEITA A LIGAÇÃO COM O CLIENT ELE VAI DAR PARA ELE UM SOCKET EM TAL POSIÇÃO
		Client[clientCount].sockID = accept(serverSocket, (struct sockaddr*) &Client[clientCount].clientAddr, &Client[clientCount].len);
		Client[clientCount].index = clientCount; //AQUI ELE VAI PEGAR O ID DO CLIENTE
		

		Client[clientCount].cliente_ip = inet_ntoa(serverAddr.sin_addr); //ARMAZENA O IP
		Client[clientCount].cliente_port = porta; //ARMAZENA A PORT
		
		
		//printf("Connection accepted from %s:%d\n", inet_ntoa(Client[clientCount].clientAddr.sin_addr), ntohs(Client[clientCount].clientAddr.sin_port));
		
		//Client[clientCount].ip = inet_ntoa(serverAddr.sin_addr);
		//Client[clientCount].port = ntohs(serverAddr.sin_port);

		pthread_create(&thread[clientCount], NULL, doNetworking, (void *) &Client[clientCount]);
		
		clientCount ++; //ADICIONA +1 PARA QUANDO O PROXIMO CLIENTE VIER, ELE PEGAR OUTRO SOCKET E OUTRO ID
		porta +=2; //ADICIONEI PARA QUE CADA NOVO CLIENTE FIQUE A 2 CASAS DE DISTANCIAS COM RELAÇÃO AS PORTAS
					// POIS EU VI QUE A SENHORA QUERIA QUE FOSSE EM CASAS DIFERENTES
					// UM OUTRO CODIGO QUE A GENTE ESTAVA MEXENDO ELE FAZIA ISSO AUTOMATICAMENTE, ENTAO TIVEMOS QUE FAZER MANUAL
 
	}

	for(int i = 0 ; i < clientCount ; i ++)
		pthread_join(thread[i],NULL);

}