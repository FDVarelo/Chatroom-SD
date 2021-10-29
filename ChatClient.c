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

void * doRecieving(void * sockID){

	int clientSocket = *((int *) sockID);
	
	while(1){

		char data[1024];
		int read = recv(clientSocket,data,1024,0);
		data[read] = '\0';
		if(strcmp(data,":0:") == 0){
			printf(" → [ID NÃO ESTÁ ONLINE.]\n");
		}else if(strcmp(data,":1:") == 0){
			printf(" → [ENVIADA COM SUCESSO.]\n");
		}else{
			printf("%s\n",data);
		}
	}

}
char tempo(){
    time_t rawtime;
    struct tm * timeinfo;
    time( &rawtime );
    timeinfo = localtime( &rawtime );
    //printf("%02d/%02d/%d %02d:%02d:%02d", timeinfo->tm_mday, timeinfo->tm_mon+1, timeinfo->tm_year+1900, timeinfo->tm_hour, timeinfo->tm_min,
    //timeinfo->tm_sec);
    return printf("[%02d/%02d/%d %02d:%02d:%02d]", timeinfo->tm_mday, timeinfo->tm_mon+1, timeinfo->tm_year+1900, timeinfo->tm_hour, timeinfo->tm_min,timeinfo->tm_sec);
}
int main(){

	int clientSocket = socket(PF_INET, SOCK_STREAM, 0);

	struct sockaddr_in serverAddr;

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(8080);
	//serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if(connect(clientSocket, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) == -1) return 0;

	printf("\t\t. . . Conexao feita com sucesso! . . .\t\t\n");
	printf("\n[?] Para enviar mensagem: enviar <ID> <mensagem>\n    Para enviar mensagem a todos: enviar 0 <mensagem>\n    Para sair: sair\n    Para listar quem esta online: listar\n\n");
	printf("\n[\t\t\t. . . CHATROOM INICIADO . . .\t\t\t]\n\n");
	
	pthread_t thread;
	pthread_create(&thread, NULL, doRecieving, (void *) &clientSocket );

	while(1){
		//printf(">>> \t");
		//doRecieving opa;
		char nome[10],desligado[10];
		char input[1024]; // enviar "ola mundo"
		scanf("%s",input);

		if(strcmp(input,"listar") == 0){

			send(clientSocket,input,1024,0);

		}else if(strcmp(input,"enviar") == 0){

			send(clientSocket,input,1024,0);
			
			scanf("%s",input);
			if(strcmp(input,"0") == 0){
				send(clientSocket,input,1024,0);

				scanf("%[^\n]s",input);
				send(clientSocket,input,1024,0); 

				printf("Mensagem enviada a todos os ID's ");
				tempo();
				printf(" → [ ENVIADA COM SUCESSO. ]\n");
			}else{
				send(clientSocket,input,1024,0);

				/*recv(clientSocket,desligado,10,0);
				if(strcmp(doRecieving,":0:") == 0 ){
					printf("[!] O cliente para o qual voce enviou ou esta offline ou nao existe!\n");
				}else{*/

				strcpy(nome,input);
				

				scanf("%[^\n]s",input);
				send(clientSocket,input,1024,0);

				printf("Mensagem enviada ao ID %s em ",nome);
				tempo();
				//printf("\n");
				//}
			}
			
		}else if(strcmp(input,"sair")==0){
			send(clientSocket,input,1024,0);
			printf("[-] Desconectado do socket.\n");
			break;
		}else{
			send(clientSocket,input,1024,0);
		}
	}


}