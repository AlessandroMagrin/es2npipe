#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define FIFO_PATH "/tmp/my_fifo"

int main() {
    // Rimuove la FIFO se esiste già da una precedente esecuzione
    unlink(FIFO_PATH);

    // Creazione della FIFO
    if (mkfifo(FIFO_PATH, 0666) == -1) {
        perror("Errore nella creazione della FIFO");
        exit(1);
    }

    printf("[SERVER] In attesa di connessioni sulla FIFO...\n");

    // Apre la FIFO in lettura.
    int fd = open(FIFO_PATH, O_RDONLY);
    if (fd == -1) {
        perror("Errore nell'apertura della FIFO in lettura");
        unlink(FIFO_PATH);
        exit(1);
    }

    char buffer[256];


    while (1) {
        ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);

        if (bytes_read > 0) {
            buffer[bytes_read] = '\0'; // Termina la stringa

            // Rimuove l'eventuale newline finale (\n) per confrontare la stringa pulita
            buffer[strcspn(buffer, "\r\n")] = '\0';

            // Se il client invia "QUIT", il server esce dal ciclo
            if (strcmp(buffer, "QUIT") == 0) {
                printf("[SERVER] Ricevuto comando QUIT. Terminazione in corso...\n");
                break;
            }

            printf("[SERVER] Ricevuto: %s\n", buffer);
        } else if (bytes_read == 0) {
            // Se tutti i client chiudono il lato di scrittura, read restituisce 0 (EOF temporaneo)
            // In una FIFO persistente, a volte si gestisce riaprendo il canale o attendendo,
            // ma in questo modello base usciamo o gestiamo la chiusura.
            continue;
        } else {
            perror("Errore in lettura");
            break;
        }
    }

    // Chiusura e pulizia finale della FIFO
    close(fd);
    unlink(FIFO_PATH);
    printf("[SERVER] Chiuso e FIFO rimossa correttamente.\n");

    return 0;
}