#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h> // Necessario per la gestione dei segnali

#define FIFO_PATH "/tmp/my_fifo"

int main() {

    // Ignoriamo SIGPIPE per evitare che il programma termini bruscamente
    // se il server si chiude mentre stiamo scrivendo sulla FIFO.
    signal(SIGPIPE, SIG_IGN);

    char buffer[256];

    printf("Inserisci un messaggio da inviare al server (digita QUIT per uscire): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        perror("Errore nella lettura da tastiera");
        return 1;
    }

   //Apertura della FIFO in scrittura
    int fd = open(FIFO_PATH, O_WRONLY);
    if (fd == -1) {
        perror("Errore nell'apertura della FIFO. Assicurati che il server sia avviato!");
        exit(1);
    }

    //Scrittura del messaggio nella FIFO
    ssize_t bytes_written = write(fd, buffer, strlen(buffer));
    if (bytes_written == -1) {
        perror("Errore di scrittura nella FIFO (forse il server è stato chiuso?)");
    } else {
        printf("Messaggio inviato con successo.\n");
    }

    // 4. Chiusura del descrittore
    close(fd);

    return 0;
}