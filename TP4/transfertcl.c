/* Transfert de fichier coté client */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/signal.h>
#include <sys/wait.h>

// Pour les sockets
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;




#define BUFFER_LEN 1024

void ftpClient(SOCKET sock);

int main(int argc, char **argv)
{
	// Variables -------
	SOCKET sock;
	char * serveur;
	int port;
	//------------------

	printf("Lancement du client FTP\n");

	if(argc < 3) {
		fprintf(stderr,"Utilisation : ./transfertcl serveur(nom ou @IP) port(numéro) \n");
		exit(0);
	}

	serveur = argv[1];
	port = atoi(argv[2]);

	printf("Serveur : %s\nPort : %d\n",serveur,port);
	printf("//--------------------------------------------------//\n");

	// Création de socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == INVALID_SOCKET)
	{
		perror("socket()");
		exit(1);
	}


	// Connexion au serveur
	struct hostent *hostinfo = NULL;
	SOCKADDR_IN sin = { 0 }; /* initialise la structure avec des 0 */
	const char *hostname = serveur;

	hostinfo = gethostbyname(hostname); /* on récupère les informations de l'hôte auquel on veut se connecter */
	if (hostinfo == NULL) /* l'hôte n'existe pas */
	{
		fprintf (stderr, "Unknown host %s.\n", hostname);
		exit(EXIT_FAILURE);
	}

	sin.sin_addr = *(IN_ADDR *) hostinfo->h_addr; /* l'adresse se trouve dans le champ h_addr de la structure hostinfo */
	sin.sin_port = htons(port); /* on utilise htons pour le port */
	sin.sin_family = AF_INET;

	if(connect(sock,(SOCKADDR *) &sin, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		perror("connect()");
		exit(2);
	}


	ftpClient(sock);

	closesocket(sock);

	return 0;
}

void readServeur(SOCKET sock){
	char buffer[BUFFER_LEN];
	int n = 0;
	if((n = recv(sock, buffer, BUFFER_LEN - 1, 0)) < 0){
		perror("recv()");
		exit(3);
	}
	buffer[n] = '\0';

	printf("# %s\n",buffer);
	printf("//--------------------------------------------------//\n");
}

void writeServeur(SOCKET sock, char* buffer){
	if(send(sock, buffer, strlen(buffer), 0) < 0)
		{
			perror("send()");
			exit(4);
		}
}



/* Corps du traitement de l'appli */
void ftpClient(SOCKET sock){

	// Variable ----
	char buffer[BUFFER_LEN];
	int n;
	int exit = 0;
	// -------------
	// Le serveur présente les possibilités
	readServeur(sock);

	// On récupère des commandes
	while(!exit) {
		
		printf("> ");
		scanf("%s", buffer);
		
		if(strcmp("quit",buffer) == 0){
			exit = 1;
		} 
		else if(strcmp("ls",buffer) == 0){
			system("ls");
		} 
		else {
			writeServeur(sock,buffer);
			readServeur(sock);
		}
		
	}

	printf("Au revoir !\n");
}
