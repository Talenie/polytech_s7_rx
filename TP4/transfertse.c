/* Transfert de fichier coté serveur */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main(int argc, char **argv)
{
	//----------------
	printf("Lancement du serveur FTP\n");

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN csin = { 0 };
	SOCKET csock;

	int sinsize = sizeof csin;

	if(sock == INVALID_SOCKET)
	{
	  perror("socket()");
	  exit(1);
	}

	SOCKADDR_IN sin = { 0 };

	sin.sin_addr.s_addr = htonl(INADDR_ANY); /* nous sommes un serveur, nous acceptons n'importe quelle adresse */

	sin.sin_family = AF_INET;

	sin.sin_port = htons(25555);

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

	csock = accept(sock, (SOCKADDR *)&csin, &sinsize);

	if(csock == INVALID_SOCKET)
	{
	    perror("accept()");
	    exit(4);
	}

	printf("Serveur connecté !");

	closesocket(sock);
	closesocket(csock);

	return 0;
}
