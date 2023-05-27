#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
/*CONSTANTS PER A DEFINIR DIRECCIO DE LA PARAULA*/
#define forward 0
#define backward 1
#define updown 2
#define downup 3

#define TIMEOUT 1700

#define MIN_LLETRES 4
#define MAX_LLETRES 8
#define MIN_PARAULES 4 // afegit extra, si no hi ha un minim de paraules no te molt sentit una sopa de lletres...
#define MAX_PARAULES 10

#include "funcions.h"

// arxiu per parametre, per accedir a la direcció cal treballar amb: argv[1]
// exemple d'execucio: ./main paraules.txt
int main(int argc, char *argv[])
{
    FILE *f;
    sopa_t sopa;
    unsigned int mida_s, mida_t, i; // mida_s: mida sopa | mida_t: mida taula de paraules
    bool validate = false;
    char **dades;

    mostra_benvinguda(); // mostra missatge de benvinguda

    // comprova si les dades d'entrada son correctes
    if (comprova_entrada(argc, argv))
    { // en el cas de que la entrada sigui la esperada i l'arxiu existeixi
        mida_t = comprova_arxiu(argv[1]);
        if (mida_t != 0){
            validate = true;
        }
            
    }

    if (validate)
    {
        dades = malloc(mida_t * sizeof(char *)); // preparem la mida de la taula de dades segons el numero de paraules que hi ha a l'arxiu
        carregar_paraules(argv[1], dades);       // carreguem les paraules a dintre d'un array
        // ordenem les paraules mitjançant un algorsime d'ordenacio: merge sort
        ordenar_paraules(dades, i, mida_t - 1); // j = mida_t - 1 ja que volem n posicions desde 0 fins a n
        mostrar_paraules(dades, mida_t);        // mostra les paraules que es troben a l'arxiu
        mida_s = demanar_mida();                // emmagatzema a la variable mida_s la mida que tindra la sopa de lletres (recordar que sera n*n)

        sopa.n_par = mida_t;               // mida_t te la dimensio de la taula de paraules, aixi que asignarem aquesta informacio dintre de el struct
        genera_sopa(dades, mida_s, &sopa); // La generem (exemple)

        mostra_sopa(sopa); // La mostrem per pantalla

       /*TO DO... tractar jugada...*/
        free(dades);
    }
    return 0;
}
