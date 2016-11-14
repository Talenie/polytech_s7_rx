/*
 * serveur.c
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
#include <curses.h>
#include <string.h>

#include <sys/signal.h>
#include <sys/wait.h>

#include "fon.h"     		/* Primitives de la boite a outils */


#define SERVICE_DEFAUT "1111"
#define PROTOCOLE_DEFAUT "tcp"
#define BUFFER_LEN 800
#define MAXHOSTNAMELEN 255

int main(int argc, char **argv)
{
	printf("Lancement du serveur ftp\n");
	serveur_appli();
	return 0;
}


/* procédure centrale de ftp */
void ftpServeur(int socket) {
	//Envoi du message de début.
	printf("hello on est dans ftpServeur \n");
	h_writes(socket, "bienvenue sur le serveur ftp\n",BUFFER_LEN);

}


/* Procedure correspondant au traitement du serveur de votre application */
void serveur_appli(){
	
	int pid = 1;
	int nbMaxRq = 10;
	char hostname[MAXHOSTNAMELEN + 1];
	int soc_serveur, soc_client; //sockets instanciées par le serveur et le client
	struct sockaddr_in *p_adr_serveur = malloc(sizeof(struct sockaddr_in));
	struct sockaddr_in *p_adr_client = malloc(sizeof(struct sockaddr_in)); //identité du client

	p_adr_serveur->sin_family = AF_INET;

	//Ouverture d'un socket
	soc_serveur = h_socket(AF_INET, SOCK_STREAM);

	
	//Récupération de l'adresse du serveur
	gethostname(hostname, MAXHOSTNAMELEN);
	adr_socket(SERVICE_DEFAUT, hostname, PROTOCOLE_DEFAUT, p_adr_serveur);

	//Binder le socket en fonction de l'adresse du serveur
	h_bind(soc_serveur, p_adr_serveur);

	//Ecoute les connexions client
	h_listen(soc_serveur, nbMaxRq);
	
	printf("hello socket listen %d\n",soc_serveur);

	//Tant que le processus courrant est la fonction principal, alors le serveur attend une demande de connexion
	while(1) {
		
		printf("hello socket while %d\n",soc_serveur);
		//Récupération du socket client
		soc_client = h_accept(soc_serveur, p_adr_client);
		printf("|===========================================|\n");
		pid = fork();
		if(pid==0)
		{
			srand(time(NULL));
			//Suppression du socket serveur
			h_close(soc_serveur);
			//Lancement du traitement ftp
			ftpServeur(soc_client);

			//Fermeture du socket client
			h_close(soc_client);

			exit(0);
		}
		else
		{
			//Fermeture du socket client
			h_close(soc_client);
		}
	}
	//Fermeture du socket serveur
	h_close(soc_serveur);
	exit(0);
}
