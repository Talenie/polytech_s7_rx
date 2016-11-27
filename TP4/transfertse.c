/* Transfert de fichier coté serveur */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libFileTrsft.c"

#include <sys/signal.h>
#include <sys/wait.h>

// Pour les sockets
#include <sys/types.h>
#include <sys/socket.h>

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

#define DOMAINE_DEFAUT "AF_INET"
#define PROTOCOLE_DEFAUT "tcp"
#define BUFFER_LEN 1024
#define MAXHOSTNAMELEN 255
#define PORT 25559

/* Corps du traitement côté serveur */
void ftpServeur(SOCKET csock) {

	// Variables ------------
	char rcv_buffer[BUFFER_LEN];
	char snd_buffer[BUFFER_LEN];
	FILE *output;

	int exit = 0;

	while(!exit) {
		// Le serveur attend le message du client
		readClient(csock, rcv_buffer);

		printf("%s \n", rcv_buffer);

		if (strcmp(rcv_buffer, "quit") == 0) {
			exit = 1;
		}
		else if(strcmp(rcv_buffer, "ls") == 0) {
			output = popen("ls", "r");
			//ATTENTION on est limités par la taille du buffer, il faut penser à mettre le résultat dans un fichier et l'envoyer par échange de fichier.
			fread(snd_buffer, sizeof(char), sizeof(snd_buffer), output);
			writeClient(csock, snd_buffer);
		}
		else if(strcmp(rcv_buffer, "put") == 0) {
			printf("%s\n", rcv_buffer);
		}
		else if (strcmp(rcv_buffer, "get") == 0) {
		}
		else {
			writeClient(csock, "Message inconnu");
		}

	}
}

int main(int argc, char **argv)
{
	//----------------
	printf("Lancement du serveur FTP\n");
	printf("Adresse du serveur : %s\n", "localhost");
	printf("Port : %d\n", PORT);

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN csin = { 0 };
	SOCKET csock;

	int sinsize = sizeof csin;

	int pid;

	if(sock == INVALID_SOCKET)
	{
	  perror("socket()");
	  exit(1);
	}

	SOCKADDR_IN sin = { 0 };

	sin.sin_addr.s_addr = htonl(INADDR_ANY); /* nous sommes un serveur, nous acceptons n'importe quelle adresse */

	sin.sin_family = AF_INET;

	sin.sin_port = htons(PORT);

	if(bind (sock, (SOCKADDR *) &sin, sizeof sin) == SOCKET_ERROR)
	{
	    perror("bind()");
	    exit(2);
	}

	if(listen(sock, 5) == SOCKET_ERROR)
	{
	    perror("listen()");
	    exit(3);
	}

	while(1) {
		csock = accept(sock, (SOCKADDR *)&csin, &sinsize);
		printf("csock =  %d\n", csock);

		if(csock == INVALID_SOCKET)
		{
			perror("accept()");
			exit(4);
		}

		pid = fork();

		if(pid == 0) {
				// Affichage de l'appli connectée
				printf("Serveur connecté !\n");

				ftpServeur(csock);

			exit(0);
		} else {
			closesocket(csock);
		}

	}

	closesocket(sock);

	return 0;
}
