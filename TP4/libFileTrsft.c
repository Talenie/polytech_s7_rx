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

void readClient(SOCKET sock, char* buffer){
	int n = 0;
	if((n = recv(sock, buffer, BUFFER_LEN - 1, 0)) < 0){
		perror("recv()");
		exit(3);
	}
	buffer[n] = '\0';
}

void writeClient(SOCKET sock, char* buffer){
	if(send(sock, buffer, strlen(buffer), 0) < 0)
		{
			perror("send()");
			exit(4);
		}
}

void sendFile(char *filename, SOCKET sock) {
  char snd_buffer[BUFFER_LEN];
  struct stat file_stat;
  char file_size[BUFFER_LEN];

  int file = open(filename, O_RDONLY);
  if(fstat(file, &file_stat) < 0) {
    perror("file_stat()");
    exit(5);
  }


  sprintf(file_size, "%d", file_stat.st_size);

  // On informe le receveur de la taille de fichier
  if(send(sock, file_size, sizeof(file_size), 0) < 0) {
		perror("send()");
		exit(4);
  }
	printf("Socket %d |	Taille du fichier à envoyer : %d o\n", sock, atoi(file_size));

  int restant = atoi(file_size); //La quantité de données restances à envoyer
  off_t decalage = 0;
  int sent;
  // Boucle d'envoi des données
  while ((restant > 0) && ((sent = sendfile(sock, file, &decalage, BUFFER_LEN)) > 0))
  {
			printf("Socket %d |	envoi de données : %d o restants\n", sock, restant);
      restant -= sent; // Calcul des données restantes à envoyer
  }

	printf("TERMINE\n");
}

void receiveFile(char *filename, SOCKET sock) {
  char rcv_buffer[BUFFER_LEN];
  int file_size;
  FILE *file;

  // taille totale du fichier à recevoir
  recv(sock, rcv_buffer, BUFFER_LEN, 0);
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

  while ((restant > 0) && ((rcv = recv(sock, rcv_buffer, BUFFER_LEN, 0)) > 0))
  {
					fwrite(rcv_buffer, sizeof(char), rcv, file); //Ecriture des données recues dans le fichier de sortie
          restant -= rcv; //Calcul des données restantes à envoyer
					printf("Socket %d |	réception de données : %d restant\n", sock, restant);
  }
  fclose(file);

}

void printRecv(SOCKET sock) {
  char rcv_buffer[BUFFER_LEN];
  int file_size;

  // taille totale du fichier à recevoir
  recv(sock, rcv_buffer, BUFFER_LEN, 0);
  file_size = atoi(rcv_buffer);

  int restant = file_size; // donnees restantes à récupérer
  int rcv; // compteur de donnees recues
  while ((restant > 0) && ((rcv = recv(sock, rcv_buffer, BUFFER_LEN, 0)) > 0))
  {
		printf("%s", rcv_buffer);
		restant -= rcv; //Calcul des données restantes à envoyer
  }

}
