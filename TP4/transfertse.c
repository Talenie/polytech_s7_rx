/* Transfert de fichier coté serveur */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/signal.h>
#include <sys/wait.h>

// Pour les sockets
#include <sys/types.h>
#include <sys/socket.h>

#define DOMAINE_DEFAUT "AF_INET"
#define PROTOCOLE_DEFAUT "tcp"
#define BUFFER_LEN 800
#define MAXHOSTNAMELEN 255

int main(int argc, char **argv)
{
	//----------------
	
	int socket_serveur;
	
	
	
	//------------------
	
	printf("Lancement du serveur FTP\n");
	
	socket_serveur = socket(DOMAINE_DEFAUT, SOCK_STREAM, int protocol);
	
	
	
	
	return 0;
}
