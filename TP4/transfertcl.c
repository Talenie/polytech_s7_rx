/* Transfert de fichier coté client */


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
#define BUFFER_LEN 1024
#define MAXHOSTNAMELEN 255

int main(int argc, char **argv)
{
	// Variables -------
	
	int sock;
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
	//~ if(sock == INVALID_SOCKET)
	//~ {
		//~ perror("socket()");
		//~ exit(1);
	//~ }
	
	
	// Connexion au serveur
	struct hostent *hostinfo = NULL;
	struct sockaddr_in *sin = malloc(sizeof(struct sockaddr_in)); /* initialise la structure avec des 0 */
	

	hostinfo = gethostbyname(serveur); /* on récupère les informations de l'hôte auquel on veut se connecter */
	if (hostinfo == NULL) /* l'hôte n'existe pas */
	{
		fprintf (stderr, "Serveur inconnu %s.\n", serveur);
		exit(EXIT_FAILURE);
	}

	sin->sin_addr = *(IN_ADDR *) hostinfo->h_addr; /* l'adresse se trouve dans le champ h_addr de la structure hostinfo */
	sin->sin_port = htons(port); /* on utilise htons pour le port */
	sin->sin_family = AF_INET;

	if(connect(sock,(SOCKADDR *) sin, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		perror("connect()");
		exit(errno);
	}
	
	return 0;
}
