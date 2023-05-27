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

/* Definicio del tipus per a desar les dades de la sopa de lletres */
/* Es una proposta que podeu canviar, ampliar, etc. */
typedef struct
{
    char ll[MAX_LLETRES + 1]; // Lletres de la paraula (comptem centinella)
    bool enc;                 // La paraula s'ha encertat
    int direccio;             // atribut definit per les constants de direccio
} paraula_t;

typedef struct
{
    int dim;                     // Nombre de files = nombre de columnes
    char *lletres;               // Taula amb les lletres
    bool *encertades;            // Les lletres son d'una paraula que s'ha encertat
    paraula_t par[MAX_PARAULES]; // Les paraules
    int **localitza_paraules;    // array a on localitzarem les lletres de les paraules generades a la sopa
    int n_par;                   // Nombre de paraules
    int n_encerts;               // Nombre de paraules encertades
} sopa_t;

//@brief fa una copia de mida identica de l'array de paraules a l'array de posicions de les lletres de les paraules
//(l'objectiu es fer que hi hagi 2 arrays equivalents en el que tinguem les paraules emmagatzemades i de forma paralela poder coneixer quina posicio ha agafat a la sopa cada lletra de la paraula)
void ini_localitzacio_sopa(sopa_t *s)
{
    short k;
    s->localitza_paraules = malloc(s->n_par * sizeof(int *)); // dimensions per a emmagatzemar les posicions de cada lletra de cada paraula
    // ara per a cada paraula generarem un altre array d'enters dintre de s->localitza_paraules
    for (k = 0; k < s->n_par; k++)
    {
        s->localitza_paraules[k] = (int *)malloc(strlen(s->par[k].ll) * sizeof(int *)); // longitud de cada paraula
    }
}

// asigna una direccio a les paraules de la taula
void defineix_direccio(sopa_t *s)
{
    int aleat, i;
    for (i = 0; i < s->n_par; i++)
    {
        s->par[i].direccio = rand() % 4;
    }
}

//@brief funcio que genera la inversa de la cadena, es a dir, organitza en sentit contrari la paraula.
void reversa_cadena(char *paraula)
{
    char *aux_paraula = malloc(strlen(paraula) + 1); // cadena auxiliar on guardaren paraula
    int d = 0, i;

    for (i = 0; paraula[i] != '\0'; i++)
    {
        aux_paraula[i] = paraula[i];
    }
    aux_paraula[i] = '\0';

    for (int len = strlen(paraula) - 1; len >= 0; len--)
    {
        paraula[d++] = aux_paraula[len]; // introduim els caracters de aux_paraula (des del darrer fins al primer) a paraula
        // (des del primer fins al darrer)
    }
    free(aux_paraula);
}

//@fa una copia del contingut de la taula dades a s->paraules i modifica encerts a cero. D'aquesta forma ja podem començar la generacio de la taula.
void preconfigura_struct(char **dades, sopa_t *s)
{
    int a, b;
    bool fi_par = false;
    char *t_par;
    // asigna direccions a les paraules
    defineix_direccio(s);
    // emmagatzemem les paraules a l'array s->par[n].ll
    for (a = 0; a < s->n_par; a++)
    {
        if (s->par[a].direccio == 0 || s->par[a].direccio == 2)
        {
            strcpy(s->par[a].ll, dades[a]);
        }
        else if (s->par[a].direccio == 1 || s->par[a].direccio == 3)
        {
            reversa_cadena(dades[a]);
            strcpy(s->par[a].ll, dades[a]);
        }
        // aprofitem per desmarcar aquestes paraules com posibles encerts
        s->par[a].enc = false;
    }
    // contador d'encerts a 0
    s->n_encerts = 0;
}

//@brief comprova que no sobrepasa el llimit de la taula, es a dir, si la taula te 100 posicions no pot tindre una paraula que comenci a la posicio 100 i acabi a la 105.
bool overflow_taula(int long_par, int rand, int mida, int mida_sopa)
{
    bool result = false;
    if (((long_par + (rand)) < mida_sopa || (long_par - (rand)) > 0))
    {
        // es possible utilitar aquesta paraula
        result = true;
    }
    return result;
}

//@brief comprova que no sobrepasa el llimit de la taula, es a dir, si la taula te 100 posicions no pot tindre una paraula que comenci a la posicio 100 i acabi a la 105.
bool overflow_taula_up_down(int long_par, int rand, int mida, int mida_sopa)
{
    bool result = false;
    if ((rand) + long_par * mida < mida_sopa)
    {
        // es possible utilitar aquesta paraula
        result = true;
    }
    return result;
}

//@brief comprova que la posicio es completament valida i no sobrepasara en cap moment el minim ni el maxim de la taula
/*
@pre example: Taula -> | | | | | | | |
              Pos ->    0 1 2 3 4 5 6
     tenim la seguent paraula: "HOLA",
     si volem inserir la paraula com maxim podrem començar a escriure la paraula desde la posicio 3 -> | | | |H|O|L|A|
     si comencem mes tard pasaria el seguent | | | | |H|O|L|, aquest cas no es pot donar, i a la inversa tampoc. (to do...)
*/
bool no_extrem_sopa(char *par, int candidat_rand, int mida, int direccio)
{
    int a;
    int posicio = candidat_rand % mida; // si per exemple tinc el numero 25 jo puc obtindre el 5 fent el modul entre 10 d'aquesta operacio
    bool no_overflow = false;
    if (overflow_taula((int)strlen(par), candidat_rand, mida, ((mida * mida))))
    {
        switch (direccio)
        {
        case 0:
            if ((((int)strlen(par) + posicio) < mida-1)) //"parche" temporal per un problema amb el salt de linea
            {
                no_overflow = true;
            }
            break;
        case 1:
             if ((((int)strlen(par) + posicio) < mida-1)) //"parche" temporal per un problema amb el salt de linea
            {
                no_overflow = true;
            }
            break;
        }
    }
    if (overflow_taula_up_down((int)strlen(par), candidat_rand, mida, ((mida * mida))))
    {
        switch (direccio)
        {
        case 2:
            if (((posicio) + (int)strlen(par) * mida < (mida * mida) - mida) && posicio >= 0)
            {
                no_overflow = true;
            }
        case 3:
            if (((posicio) + (int)strlen(par) * mida < (mida * mida) - mida) && posicio >= 0)
            {
                no_overflow = true;
            }
            break;
        }
    }
    return no_overflow;
}

//@brief petita funció que genera un error de TIMEOUT en el cas de que no ha pogut completar alguna accio (exemple: no hi ha lloc per una paraula)
void timeout(int count, bool *exception)
{
    if (count >= (int)TIMEOUT)
    {
        *exception = true;
    }
}

//@brief retorna un valor aleatori seguint les instruccions donades de limits i no repeticio per a la sopa
int genera_aleatori(sopa_t *s, char *paraula, int rand_num, int mida, int dir)
{
    short iterate = 0;
    bool fin;
    do
    {
        rand_num = rand() % ((int)strlen(s->lletres)); // retorna un numero aleatori respectant les posicions maximes indicades a la taula
        if (no_extrem_sopa(paraula, rand_num, mida, s->par[dir].direccio))
        {
            fin = true;
        }
        else
        {
            fin = false;
        }
        iterate++;
        if (iterate == TIMEOUT)
        {
            fin = true;
        }

    } while (!fin);
    return rand_num;
}




//@brief volem comprovar que cap lletra es "trepitja" amb alguna altra previament especificada
bool comprova_aleatoris_existents_horizontal(sopa_t *s, char *paraula, int inici_par, int mida)
{
    int i, j, k, fi_par = ((int)strlen(paraula) + inici_par);
    bool valid = true;

    for (i = 0; i < s->n_par; i++)
    {
        for (j = 0; j < (int)strlen(s->par[i].ll); j++)
        {
            for(k = inici_par; k <= fi_par; k++){
                if(s->localitza_paraules[i][j] == k){
                    valid = false;      // comprova que l'aleatori seleccionat no afecta a cap altre aleatori existent
                }
            }
            if(inici_par%mida == 0){     //comprobar dirección para eviar en el caso de que vayan hacia atraás que sea 0 el rand
                valid = false;
                
            }
            
           
        }
    }
    return valid;
}

bool comprova_aleatoris_existents_vertical(sopa_t *s, char *paraula, int inici_par, int mida)
{
    int i, j, k;
    bool valid = true;
    int fi_par = inici_par + mida * (int)strlen(paraula);
    for (i = 0; i < s->n_par; i++)
    {
        for (j = 0; j < (int)strlen(s->par[i].ll); j++)
        {
            
            for(k = inici_par; k <= fi_par; k++){
                if(s->localitza_paraules[i][j] == k && !(s->par[i].ll[0] == paraula[0])){
                    valid = false;      // comprova que l'aleatori seleccionat no afecta a cap altre aleatori existent
                }
            }
            
        }
    }
    return valid;
}

/* Mostra una lletra segons si pertany a encert o no. No caldria modificar */
void mostra_lletra(char ll, bool enc)
{
    if (enc)
    {
        printf("\033[0;42m"); // Color verd de fons
        printf(" %c", ll);
        printf("\033[0m"); // Tornem al color per defecte
    }
    else
    {
        printf(" %c", ll);
    }
}

/* Mostra la sopa de lletres pel terminal */
/* En principi, NO HAURIEU DE MODIFICAR AQUEST CODI */
void mostra_sopa(sopa_t s)
{
    // Mostrem els numeros de columna
    printf("\033[0;31m"); // Color
    printf("  ");

    for (int i = 10; i < s.dim + 1; i += 10)
    {
        for (int j = 0; j < 18; j++)
            printf(" ");
        printf(" %d", i / 10);
    }

    printf("\n  ");
    for (int i = 0; i < s.dim; i++)
    {
        int p = (i % 10) + 1;
        p != 10 ? printf(" %d", p) : printf(" 0");
    }

    printf("\n");
    printf("\033[0m"); // Tornem al color per defecte

    // Mostrem les lletres. Cada lletra ocupa dos espais:
    // Si correspon a un encert, es marca
    for (int i = 0; i < s.dim; i++)
    {
        printf("\033[0;31m");  // Color
        printf("%-2d", i + 1); // Mostrar numero de linia
        printf("\033[0m");     // Tornem al color per defecte

        for (int j = 0; j < s.dim; j++)
        {
            if (s.encertades[i * s.dim + j])
            {
                printf("\033[0;42m"); // Color verd de fons
                printf(" %c", s.lletres[i * s.dim + j]);
                printf("\033[0m"); // Tornem al color per defecte
            }
            else
            {
                printf(" %c", s.lletres[i * s.dim + j]);
            }
        }
        printf("\n");
    }
    printf("\n");

    printf("Portes %d encerts.\n", s.n_encerts);
    printf("Paraules a trobar: %d\n", s.n_par - s.n_encerts);
    for (int i = 0; i < s.n_par; i++)
    {
        if (!s.par[i].enc)
            printf("%s\n", s.par[i].ll);
    }
}









void generar_posicions_aleatories(sopa_t *s, int mida, char **posicions)
{
    int b = 0, c, d, e = 0, i = 0, long_par, posicio = 0;
    short word = 0;
    bool fin, exception = false, not_complete = false;
    int rand_num;

    for (d = 0; d < s->n_par; d++)
    { // itera sobre paraules a sopa_t
        
        b = 0; // contador error
        if (s->par[d].direccio == 0 || s->par[d].direccio == 1)
        {
            do
            {
                rand_num = genera_aleatori(s, s->par[d].ll, rand_num, mida, d) - 1; // retorna un numero aleatori respectant les posicions maximes indicades a la taula
                timeout(b, &exception);
                b++;
            } while (!comprova_aleatoris_existents_horizontal(s, s->par[d].ll, rand_num, mida) && !exception);

            if (!exception)
            {
                for (e = 0; e < ((int)strlen(s->par[d].ll)); e++)
                {
                    s->localitza_paraules[d][e] = rand_num; // posicions que anira omplint
                    rand_num++;                             // com aquest aleatori indicara la posicio a on arranca la paraula generada les seguents posicions seran les que contindran la resta de lletres de la paraula
                    s->lletres[rand_num] = s->par[d].ll[e];
                    s->encertades[(int)s->localitza_paraules[d][e] + 1] = true;
                    //printf("\n%c -> %d", s->par[d].ll[e], s->localitza_paraules[d][e]);
                }
            }else{
                printf("\nLa paraula '%s' no s'ha pogut ubicar", s->par[d].ll);
                exception = false;
                not_complete = true;
            }

        }
        else if (s->par[d].direccio == 2 || s->par[d].direccio == 3)
        {
            do
            {
                rand_num = genera_aleatori(s, s->par[d].ll, rand_num, mida, d); // retorna un numero aleatori respectant les posicions maximes indicades a la taula
                timeout(b, &exception);
                b++;
            } while (!comprova_aleatoris_existents_vertical(s, s->par[d].ll, rand_num, mida) && !exception);
            if (!exception)
            {
                s->localitza_paraules[d][0] = rand_num; // posicions que anira omplint
                posicio = rand_num;
                for (i = 0; i < ((int)strlen(s->par[d].ll)); i++)
                {
                    s->localitza_paraules[d][i] = posicio;
                    s->lletres[posicio] = s->par[d].ll[i];
                    s->encertades[(int)s->localitza_paraules[d][i]] = true;
                    posicio += mida;
                    //printf("\n%c -> %d", s->par[d].ll[i], s->localitza_paraules[d][i]);
                }
            }else{
                 printf("\nLa paraula '%s' no s'ha pogut ubicar", s->par[d].ll);
                 exception = false;
                 not_complete = true;
            }
        }
    }
    printf("\n");
    if(not_complete){
        printf("\nHi ha hagut un problema per a generar totes les paraules, només s'han pogut generar les paraules especificades\n");
    }
}

/* Aquesta funcio genera la sopa de lletres, a partir del fitxer i altres parametres */
/* que ja decidireu. En aquest cas només l'emplena amb una SOPA d'EXEMPLE, es a dir */
/* que haureu de fer la vostra pròpia. */
void genera_sopa(char **dades, int mida, sopa_t *s)
{
    srand(time(NULL)); // seed per a poder treballar amb aleatoris
    char **posicions;

    s->dim = mida;                                          // Mida màxima: 40 x 40
    s->lletres = malloc(s->dim * s->dim * sizeof(char));    // Espai per a les lletres
    s->encertades = malloc(s->dim * s->dim * sizeof(char)); // Per saber si una lletra correspon a encert

    // genera les lletres de la sopa de lletres
    for (int i = 0; i < s->dim * s->dim; i++)
    {
        s->encertades[i] = false;
        // Generem una lletra aleatoriament
        s->lletres[i] = 'A' + (rand() % ('Z' - 'A'));
    }

    preconfigura_struct(dades, s); // set de dades de sopa_t per iniciar partida
    ini_localitzacio_sopa(s);      // prepara una taula a on emmagatzemara les dades de cada paraula a la sopa generada

    // TO DO...
    //- inserir paraules de l'array a la cadena de text de forma completament aleatoria
    generar_posicions_aleatories(s, mida, posicions);
}


//@brief funcio que demana la mida que tindra la taula, aquesta mida sera n * n
int demanar_mida()
{
    int mida;
    do
    {
        printf("\n\nIntrodueix la mida de la sopa de lletres. Recorda que és un cuadrat de costat mínim 10 i màxim 40.\nMida: ");
        scanf("%i", &mida);
    } while (mida < 10 || mida > 40);
    return mida;
}

//@brief mostra missatge de enhorabona al acabar el joc guanyant
void mostra_enhorabona()
{
    printf("Enhorabona! Has resolt la sopa de lletres.\n");
}

//@brief funcio que comprova el missatge que l'usuari ingresa a el joc, en el cas de que sigui la paraula 'RENDICIO' dona per finalitzada la partida
bool rendicio(char *paraula)
{
    int i = 0, p, r;
    bool res = true;
    char rendicio[MAX_LLETRES + 1] = {'R', 'E', 'N', 'D', 'I', 'C', 'I', 'O', '\0'};
    p = strlen(paraula);
    r = strlen(rendicio);
    if (p != r)
    {
        res = false;
    }
    else
    {
        while (i < 8 && res)
        {
            if (paraula[i] != rendicio[i])
            {
                res = false;
            }
            i++;
        }
    }
    return res;
}

//@brief funcio que revela la sopa quan l'usuari es rendeix
void revelar_sopa(sopa_t s)
{
    int b;
    /*for(b = 0; b < s.dim * s.dim; b++){
         s.par[b].enc = true;
    } */
    // TO DO... (fins a que no hi hagi una funció que generi la sopa especifica aquesta no es pot completar correctament)

    mostra_sopa(s); // mostra la sopa
}

void mostra_benvinguda()
{
    printf("\n******************************************************\nUs donem la benvinguda al joc de la sopa de lletres!\nAutors: Joel, Ruben, Alberto\n******************************************************\n\n");
}

bool comprova_entrada(int n_params, char *params[])
{
    bool check = false, open = false;
    FILE *exist = fopen(params[1], "r");
    if (n_params < 2)
    { // param 2 = ruta arxiu
        printf("\nComprova que has indicat la ruta d'un arxiu de paraules!");
    }
    else
    {
        if (exist != NULL)
        {
            check = true;
            fclose(exist);
        }
        else
        {
            printf("\nComprova que has indicat una ruta a un arxiu correcte!");
        }
    }
    return check;
}

//@brief funcio que comprova si la longitud de la paraula es major al maxim especificat, i retorna a l'usuari un missatge en cas d'error
bool comprova_paraula(unsigned int mida)
{
    bool valid = true;
    if (mida > MAX_LLETRES)
    {
        printf("\nL'arxiu especificat conté paraules que sobrepasen el limit de lletres");
        valid = false;
    }
    else if (mida < MIN_LLETRES)
    {
        printf("\nL'arxiu especificat conté paraules que no respeten el limit de lletres");
        valid = false;
    }
    return valid;
}

//@brief funcio que comprova la quantitat de paraules que hi ha a l'arxiu
unsigned int comprova_arxiu(char *route)
{
    char p_temp[100]; // per no tindre problemes amb paraules molt llargues tenim un màxim de 100 chars disponibles per llegir les dades, si per alguna rao es especifica una paraula amb mes caracters el programa retornara un error de procesament 'smash stack error'.
    bool arxiuValid = true;
    unsigned int count_paraula = 0;
    FILE *t_file = fopen(route, "r");
    if (t_file != NULL)
    {
        while (fscanf(t_file, "%s", p_temp) != EOF)
        {
            if (comprova_paraula(strlen(p_temp)))
            { // conta les paraules de l'arxiu mentres fa una cerca de possibles paraules invalides, si aixo es compleix invalida l'arxiu
                count_paraula++;
            }
            else
            {
                arxiuValid = false;
            }
        }
        fclose(t_file);
    }
    if (!arxiuValid)
        count_paraula = 0; // reinicia contador de paraules ja que invalidem tot l'arxiu
    return count_paraula;
}

//@brief funcio que carrega les paraules de l'arxiu en un array de paraules que compleixen les regles de mida a la memoria optimes.
bool carregar_paraules(char *route, char **dades)
{
    unsigned int i = 0;
    char t_str[MAX_LLETRES - 1];      // variable temporal a on emmagatzemare les paraules de l'arxiu
    char *p_temp;                     // variable a on emmagatzemare les paraules de la variable t_var pero amb la mida correcta
    FILE *t_file = fopen(route, "r"); // com previament ja he pogut obrir l'arxiu ara no hauria de tenir cap de problema...
    while (fscanf(t_file, "%s", t_str) != EOF)
    {
        // p_temp = (char*) malloc(strlen(t_str) * sizeof(char*));   //anotacio: sizeof(char*) -> la mida d'un char es d'1 byte...
        dades[i] = (char *)malloc(strlen(t_str) * sizeof(char *)); // mida de cada paraula a dintre de l'array principal (bidimensional)
        strcpy(dades[i], t_str);                                   // emmagatzema a l'array dades cada camp de l'arxiu, cada camp te la mida necesaria i no mes per estalviar espai
        i++;
    }
    fclose(t_file);
    return false;
}

//@brief s'ocupa de jugar amb les posicions de l'array i acaba ordenant a la taula pasada per referencia mitjançant una taula temporal
void mescla(char **dades, int i, int m, int j)
{
    char *temp[j];
    int pos_left = i, pos_right = m + 1, k;
    k = i;
    // Mentre hi ha dades a les dues porcions
    while ((pos_left <= m) && (pos_right <= j))
    { // Posem el menor i avancem
        // printf("\n%s",dades[pos_left]);
        // printf("\n%s",dades[pos_right]);
        if (strcmp(dades[pos_left], dades[pos_right]) < 0)
        { // dades[left] es mes petit que dades[right]
            temp[k] = dades[pos_left];
            pos_left++;
        }
        else
        { // dades[left] es mes gran que dades[right] O son strings completament iguals, en aquest cas dona igual l'ordre aixi que quedara aqui
            temp[k] = dades[pos_right];
            pos_right++;
        }
        k++;
    }
    // a aquest punt organitzem les posicions a l'array temporal
    while (pos_right <= j)
    {
        temp[k] = dades[pos_right];
        pos_right++;
        k++;
    }
    while (pos_left <= m)
    {
        temp[k] = dades[pos_left];
        pos_left++;
        k++;
    }
    // Fent una interacio sobre l'array principal tornem a copiar les dades desde l'array temporal
    for (k = i; k <= j; k++)
    {
        dades[k] = (char *)malloc(strlen(temp[k]) * sizeof(char *)); // com reorganitzem les paraules aquestes posicions de l'array no tenen la mida correcta
        dades[k] = temp[k];
    }
}

// decisio de disseny: algoritme de ordenacio mergesort, molt eficient amb grans quantitats de dades (en aquest cas no son quantitats molt grans pero haura d'iterar sobre cada lletra de les paraules)
void ordenar_paraules(char **dades, int i, int j)
{
    unsigned int m;
    if (j != i)
    {
        m = (i + j) / 2;                   // valor a la meitat (frontera)
        ordenar_paraules(dades, i, m);     // Meitat esquerra
        ordenar_paraules(dades, m + 1, j); // Meitat dreta
        mescla(dades, i, m, j);
    }
}
void mostrar_paraules(char **dades, unsigned int mida_t)
{
    printf("\nHi ha un total de %d paraules:\n", mida_t);
    for (int a = 0; a < mida_t; a++)
    {
        printf("%s\n", dades[a]); // a dades hi ha les paraules de l'arxiu
    }
}