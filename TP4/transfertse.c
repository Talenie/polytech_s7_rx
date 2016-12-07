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
#define PORT 25564

/* Corps du traitement côté serveur */
void ftpServeur(SOCKET csock) {

	// Variables ------------
	char rcv_buffer[BUFFER_LEN];
	char snd_buffer[BUFFER_LEN];
	//FILE *output;

	int exit = 0;

	while(!exit) {
		// Le serveur attend le message du client
		readClient(csock, rcv_buffer);

		printf("Socket %d |	commande recue : %s \n", csock, rcv_buffer);

		/* Découpage de la commande reçue */
		char * token;
	  token = strtok (rcv_buffer," ");

		if(token != NULL) {
			if (strcmp(token, "quit") == 0) {
				exit = 1;
			}
			else if(strcmp(token, "ls") == 0) {
				// seul problème, on voit lsse dans le ls
				system("ls > lsse.tmp");
				sendFile("lsse.tmp", csock);
				//suppression du fichier temporaire
				remove("lsse.tmp");
			}
			else if(strcmp(token, "put") == 0) {
				token = strtok(NULL, " ");
				while (token != NULL)
				{
					printf("%s\n", token);

					receiveFile(token, csock);

					token = strtok(NULL, " ");
				}
			}
			else if (strcmp(token, "get") == 0) {
				token = strtok(NULL, " ");
				while (token != NULL)
				{
					printf("%s\n", token);

					sendFile(token, csock);

					token = strtok(NULL, " ");
				}
			}
			else {
				writeClient(csock, "Message inconnu");
			}
		} else {
			writeClient(csock, "Message vide");
		}

	}

	printf("Socket %d |	Fermeture de la session.\n", csock);
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
