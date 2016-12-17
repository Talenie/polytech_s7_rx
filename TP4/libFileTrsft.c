#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/signal.h>
#include <sys/wait.h>

// Pour les sockets
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/sendfile.h>

typedef int SOCKET;

#define BUFFER_LEN 1024

/**
 * Fonction permettant au serveur de récupérer les commandes émises par le client
 */
void readServeur(SOCKET sock){
	char buffer[BUFFER_LEN];
	int n = 0;
	while((n = recv(sock, buffer, BUFFER_LEN - 1, 0)) < 0){
		perror("recv()");
		exit(3);
	}
	buffer[n] = '\0';

	printf("# %s\n",buffer);
	printf("//--------------------------------------------------//\n");
}

/**
 * Fonction d'écriture du serveur vers le client
 */
void writeServeur(SOCKET sock, char* buffer){
	while(send(sock, buffer, strlen(buffer), 0) < 0) // envoi unique avec vérification d'erreur
		{
			perror("send()");
			exit(4);
		}
}

/**
 * Fonction de lecture côté client
 */
void readClient(SOCKET sock, char* buffer){
	int n = 0;
	while((n = recv(sock, buffer, BUFFER_LEN - 1, 0)) < 0){
		perror("recv()");
		exit(3);
	}
	buffer[n] = '\0';
}

/**
 * Fonction d'écriture du client vers le serveur
 */
void writeClient(SOCKET sock, char* buffer){
	while(send(sock, buffer, strlen(buffer), 0) < 0)
		{
			perror("send()");
			exit(4);
		}
}

/**
 * Fonction d'envoi de fichier
 */
void sendFile(char *filename, SOCKET sock) {
  struct stat file_stat; // stockage des informations relatives au fichier
  char file_size[BUFFER_LEN];

  int file = open(filename, O_RDONLY);
  if(fstat(file, &file_stat) < 0) { // récupération des informations sur le fichier à envoyer
    perror("file_stat()");
    exit(5);
  }

  sprintf(file_size, "%d", file_stat.st_size); // la taille du fichier est stockée dans file_size

  // On informe le receveur de la taille de fichier
  while(send(sock, file_size, sizeof(file_size), 0) < 0) {
		perror("send()");
		exit(4);
  }
	printf("Socket %d |	Taille du fichier à envoyer : %d o\n", sock, atoi(file_size));

  int restant = atoi(file_size); //La quantité de données restances à envoyer
  off_t decalage = 0;
  int sent;
  // Boucle d'envoi des données
  while ((restant > 0) && ((sent = sendfile(sock, file, &decalage, BUFFER_LEN-1)) > 0))
  {
			printf("Socket %d |	envoi de données : %d o restants\n", sock, restant);
      restant -= sent; // Calcul des données restantes à envoyer
  }

	close(file);

}

/**
 * Fonction de réception de fichier
 */
void receiveFile(char *filename, SOCKET sock) {
  char rcv_buffer[BUFFER_LEN];
  int file_size;
  FILE *file;

  // taille totale du fichier à recevoir
  while(recv(sock, rcv_buffer, BUFFER_LEN, 0)<0);
  file_size = atoi(rcv_buffer);
	printf("Taille du fichier : %d o\n", file_size);

  file = fopen(filename, "w");
  if (file == NULL)
  {
          perror("fopen()");
          exit(6);
  }

  int restant = file_size; // donnees restantes à récupérer
  int rcv; // compteur de donnees recues

  while ((restant > 0) && ((rcv = recv(sock, rcv_buffer, BUFFER_LEN, 0)) > 0)) // réception des données par bloc de taille BUFFER_LEN
  {
					fwrite(rcv_buffer, sizeof(char), rcv, file); //Ecriture des données recues dans le fichier de sortie
          restant -= rcv; //Calcul des données restantes à envoyer
					printf("Socket %d |	réception de données : %d restant\n", sock, restant);
  }
  fclose(file);
	printf("fichier %s reçu.\n", filename);

}


/**
 * Fonction de lecture de fichier reçu (sans sauvegarde donc)
 */
void printRecv(SOCKET sock) {
  char rcv_buffer[BUFFER_LEN];
  int file_size;

  // taille totale du fichier à recevoir
  while(recv(sock, rcv_buffer, BUFFER_LEN, 0)<0);
  file_size = atoi(rcv_buffer);

  int restant = file_size; // donnees restantes à récupérer
  int rcv; // compteur de donnees recues
  while ((restant > 0) && ((rcv = recv(sock, rcv_buffer, BUFFER_LEN, 0)) > 0))
  {
		printf("%s", rcv_buffer);
		restant -= rcv; //Calcul des données restantes à envoyer
  }

}
