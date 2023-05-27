
#include "funcions.c"

//@brief fa una copia de mida identica de l'array de paraules a l'array de posicions de les lletres de les paraules
//(l'objectiu es fer que hi hagi 2 arrays equivalents en el que tinguem les paraules emmagatzemades i de forma paralela poder coneixer quina posicio ha agafat a la sopa cada lletra de la paraula)
void ini_localitzacio_sopa(sopa_t *s);

// asigna una direccio a les paraules de la taula
void defineix_direccio(sopa_t *s);

//@brief funcio que genera la inversa de la cadena, es a dir, organitza en sentit contrari la paraula.
void reversa_cadena(char *paraula);

//@fa una copia del contingut de la taula dades a s->paraules i modifica encerts a cero. D'aquesta forma ja podem començar la generacio de la taula.
void preconfigura_struct(char **dades, sopa_t *s);

//@brief comprova que no sobrepasa el llimit de la taula, es a dir, si la taula te 100 posicions no pot tindre una paraula que comenci a la posicio 100 i acabi a la 105.
bool overflow_taula(int long_par, int rand, int mida, int mida_sopa);

//@brief comprova que no sobrepasa el llimit de la taula, es a dir, si la taula te 100 posicions no pot tindre una paraula que comenci a la posicio 100 i acabi a la 105.
bool overflow_taula_up_down(int long_par, int rand, int mida, int mida_sopa);

//@brief comprova que la posicio es completament valida i no sobrepasara en cap moment el minim ni el maxim de la taula
/*
@pre example: Taula -> | | | | | | | |
              Pos ->    0 1 2 3 4 5 6
     tenim la seguent paraula: "HOLA",
     si volem inserir la paraula com maxim podrem començar a escriure la paraula desde la posicio 3 -> | | | |H|O|L|A|
     si comencem mes tard pasaria el seguent | | | | |H|O|L|, aquest cas no es pot donar, i a la inversa tampoc. (to do...)
*/
bool no_extrem_sopa(char *par, int candidat_rand, int mida, int direccio);

//@brief petita funció que genera un error de TIMEOUT en el cas de que no ha pogut completar alguna accio (exemple: no hi ha lloc per una paraula)
void timeout(int count, bool *exception);

//@brief retorna un valor aleatori seguint les instruccions donades de limits i no repeticio per a la sopa
int genera_aleatori(sopa_t *s, char *paraula, int rand_num, int mida, int dir);



//@brief volem comprovar que cap lletra es "trepitja" amb alguna altra previament especificada
bool comprova_aleatoris_existents_horizontal(sopa_t *s, char *paraula, int inici_par, int mida);

bool comprova_aleatoris_existents_vertical(sopa_t *s, char *paraula, int inici_par, int mida);

/* Mostra una lletra segons si pertany a encert o no. No caldria modificar */
void mostra_lletra(char ll, bool enc);

/* Mostra la sopa de lletres pel terminal */
/* En principi, NO HAURIEU DE MODIFICAR AQUEST CODI */
void mostra_sopa(sopa_t s);


void generar_posicions_aleatories(sopa_t *s, int mida, char **posicions);

/* Aquesta funcio genera la sopa de lletres, a partir del fitxer i altres parametres */
/* que ja decidireu. En aquest cas només l'emplena amb una SOPA d'EXEMPLE, es a dir */
/* que haureu de fer la vostra pròpia. */
void genera_sopa(char **dades, int mida, sopa_t *s);


//@brief funcio que demana la mida que tindra la taula, aquesta mida sera n * n
int demanar_mida();

//@brief mostra missatge de enhorabona al acabar el joc guanyant
void mostra_enhorabona();

//@brief funcio que revela la sopa quan l'usuari es rendeix
void revelar_sopa(sopa_t s);
void mostra_benvinguda();

bool comprova_entrada(int n_params, char *params[]);
//@brief funcio que comprova si la longitud de la paraula es major al maxim especificat, i retorna a l'usuari un missatge en cas d'error
bool comprova_paraula(unsigned int mida);

//@brief funcio que comprova la quantitat de paraules que hi ha a l'arxiu
unsigned int comprova_arxiu(char *route);
//@brief funcio que carrega les paraules de l'arxiu en un array de paraules que compleixen les regles de mida a la memoria optimes.
bool carregar_paraules(char *route, char **dades);

//@brief s'ocupa de jugar amb les posicions de l'array i acaba ordenant a la taula pasada per referencia mitjançant una taula temporal
void mescla(char **dades, int i, int m, int j);

// decisio de disseny: algoritme de ordenacio mergesort, molt eficient amb grans quantitats de dades (en aquest cas no son quantitats molt grans pero haura d'iterar sobre cada lletra de les paraules)
void ordenar_paraules(char **dades, int i, int j);
void mostrar_paraules(char **dades, unsigned int mida_t);