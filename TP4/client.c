/*
 * client.c
 * 
 * ABONNENC
 * BONHOURE
 * CHAMBONNET
 * 
 * RICM4
 * 
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <curses.h> 		/* Primitives de gestion d'�cran */
#include <sys/signal.h>
#include <sys/wait.h>

#include "fon.h"   		/* primitives de la boite a outils */
 

#define SERVICE_DEFAUT "1111"
#define PROTOCOLE_DEFAUT "tcp"
#define SERVEUR_DEFAUT "localhost"
#define BUFFER_LEN 800
#define MAXHOSTNAMELEN 255

void client_appli (char *serveur);

int main(int argc, char **argv)
{
	char *serveur= SERVEUR_DEFAUT; /* serveur par defaut */
	char *service= SERVICE_DEFAUT; /* numero de service par defaut (no de port) */
	char *protocole= PROTOCOLE_DEFAUT; /* protocole par defaut */

	/* Permet de passer un nombre de parametre variable a l'executable */
	switch(argc)
	{
 	case 1 :		/* arguments par defaut */
		  printf("serveur par defaut: %s\n",serveur);
		  printf("service par defaut: %s\n",service);
		  printf("protocole par defaut: %s\n",protocole);
		  break;
  	case 2 :		/* serveur renseigne  */
		  serveur=argv[1];
		  printf("service par defaut: %s\n",service);
		  printf("protocole par defaut: %s\n",protocole);
		  break;
	default:
		  printf("Usage:client serveur(nom ou @IP)\n");
		  return 1;
	}

	/* serveur est le nom (ou l'adresse IP) auquel le client va acceder */
	/* service le numero de port sur le serveur correspondant au  */
	/* service desire par le client */
	/* protocole le protocole qui sera utilise pour la communication */

	client_appli(serveur);
	
	return 0;
}


/* procedure correspondant au traitement du client de votre application */
void client_appli (char *serveur)
{

  int soc_client;
  int mode;
	char hostname[MAXHOSTNAMELEN + 1];

	struct sockaddr_in *p_adr_serveur = malloc(sizeof(struct sockaddr_in));
	struct sockaddr_in *p_adr_local = malloc(sizeof(struct sockaddr_in));

	p_adr_serveur->sin_family = AF_INET;

  // Initialisation du mode
	mode = SOCK_DGRAM;

	// Création du socket
	soc_client = h_socket(PF_INET,mode);

	gethostname(hostname,MAXHOSTNAMELEN);
	adr_socket(SERVICE_DEFAUT,hostname,PROTOCOLE_DEFAUT, p_adr_local);

	while(1)
	{
		adr_socket(SERVICE_DEFAUT,hostname,PROTOCOLE_DEFAUT, p_adr_serveur);//remplit p_adr_socket
		//CONNECT
		h_connect(soc_client,p_adr_serveur);
		//Appli
		ftpClient(soc_client);
		//CLOSE CLIENT
		h_close(soc_client);
		exit(0);
		break;
	}
 }
 
 
void ftpClient(int socket) {

	//Buffer de réception
	char buffer[800];

	printf("|===========================================|\n");
	//Affichage du début de la partie.
	h_reads(socket,buffer, BUFFER_LEN);
	printf("%s", buffer);
}

