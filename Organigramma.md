    Producers
    a. Creazione di 3 produttori: driver, military e material
    b. Ogni produttore attende il semaforo corrispondente (sem_driver, sem_military o sem_material)
    c. Una volta ottenuto il semaforo, il produttore chiama la funzione put(weight, range, col_index) con i parametri appropriati
    d. La funzione put:
    i. Trova la prima riga vuota nella colonna corrispondente nel buffer
    ii. Aggiunge il peso generato casualmente alla riga e alla colonna corrispondenti
    iii. Se la riga è completa (tutte e tre le colonne hanno valori diversi da zero), calcola il peso totale del convoglio e incrementa l'ID del convoglio
    iv. In base al peso totale, invia un segnale al semaforo corrispondente (sem_plane, sem_truck o sem_boat)
    v. Rilascia il semaforo corrispondente al produttore

    Consumers
    a. Creazione di 3 consumatori: plane, truck e boat
    b. Ogni consumatore attende il semaforo corrispondente (sem_plane, sem_truck o sem_boat)
    c. Una volta ottenuto il semaforo, il consumatore chiama la funzione get(const char *name) con il nome appropriato
    d. La funzione get:
    i. Trova il convoglio appropriato nel buffer in base al peso totale e all'ID del convoglio
    ii. Se un convoglio corrispondente è stato trovato, azzera i valori nella riga del buffer e incrementa il conteggio dei convogli processati
    iii. Rilascia i semafori corrispondenti ai produttori (sem_driver, sem_military e sem_material)

Questo processo continua fino a quando non viene raggiunto il numero massimo di convogli processati (NUM_CONVOIS).