#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MIN_LLETRES 4
#define MAX_LLETRES 8
#define MIN_PARAULES 4      //afegit extra, si no hi ha un minim de paraules no te molt sentit una sopa de lletres...
#define MAX_PARAULES 10

/* Definicio del tipus per a desar les dades de la sopa de lletres */
/* Es una proposta que podeu canviar, ampliar, etc. */
typedef struct 
{
    char ll[MAX_LLETRES + 1];   // Lletres de la paraula (comptem centinella)
    bool enc;   // La paraula s'ha encertat
} paraula_t;

typedef struct 
{
    int dim;        // Nombre de files = nombre de columnes
    char *lletres;  // Taula amb les lletres
    bool *encertades;   // Les lletres son d'una paraula que s'ha encertat
    paraula_t par[MAX_PARAULES];    // Les paraules
    int n_par;  // Nombre de paraules
    int n_encerts;    // Nombre de paraules encertades
} sopa_t;

/* Aquesta funcio genera la sopa de lletres, a partir del fitxer i altres parametres */
/* que ja decidireu. En aquest cas només l'emplena amb una SOPA d'EXEMPLE, es a dir */
/* que haureu de fer la vostra pròpia. */
void genera_sopa(sopa_t *s)
{
    s->dim = 30;    // Mida màxima: 40 x 40
    s->lletres = malloc(s->dim * s->dim * sizeof(char));   // Espai per a les lletres
    s->encertades = malloc(s->dim * s->dim * sizeof(char)); // Per saber si una lletra correspon a encert
    for (int i = 0; i < s->dim * s->dim; i++)
    {
        s->encertades[i] = false;
        // Generem una lletra aleatoriament
        s->lletres[i] = 'A' + (rand() % ('Z'-'A'));
    }

    s->n_par = 5;
    strcpy(s->par[0].ll, "ALZINA");
    strcpy(s->par[1].ll, "ARBUST");
    strcpy(s->par[2].ll, "BOLET");
    strcpy(s->par[3].ll, "CAMI");
    strcpy(s->par[4].ll, "PEDRA");

    s->n_encerts = 2;
    s->par[0].enc = false;
    s->par[1].enc = true;
    s->par[2].enc = true;
    s->par[3].enc = false;
    s->par[4].enc = false;

    // Ara posem un parell de paraules a la sopa com si s'haguessin encertat
    s->lletres[5] = 'B'; s->encertades[5] = true;
    s->lletres[6] = 'O'; s->encertades[6] = true;
    s->lletres[7] = 'L'; s->encertades[7] = true;
    s->lletres[8] = 'E'; s->encertades[8] = true;
    s->lletres[9] = 'T'; s->encertades[9] = true;

    s->lletres[65 + s->dim] = 'A'; s->encertades[65 + s->dim] = true;
    s->lletres[65 + 2 * s->dim] = 'R'; s->encertades[65 + 2 * s->dim] = true;
    s->lletres[65 + 3 * s->dim] = 'B'; s->encertades[65 + 3 * s->dim] = true;
    s->lletres[65 + 4 * s->dim] = 'U'; s->encertades[65 + 4 * s->dim] = true;
    s->lletres[65 + 5 * s->dim] = 'S'; s->encertades[65 + 5 * s->dim] = true;
    s->lletres[65 + 6 * s->dim] = 'T'; s->encertades[65 + 6 * s->dim] = true;
    
}


/* Mostra una lletra segons si pertany a encert o no. No caldria modificar */
void mostra_lletra(char ll, bool enc)
{
    if (enc) 
    {
        printf("\033[0;42m");   // Color verd de fons
        printf(" %c", ll);
        printf("\033[0m");  // Tornem al color per defecte
    }
    else
    {
        printf(" %c", ll);
    }
    
            
}


/* Mostra la sopa de lletres pel terminal */
/* En principi, NO HAURIEU DE MODIFICAR AQUEST CODI */
void mostra_sopa (sopa_t s)
{
    // Mostrem els numeros de columna
    printf("\033[0;31m");   // Color 
    printf("  ");
    for (int i = 10; i < s.dim + 1; i+=10)
    {
        for (int j=0; j < 18; j++)
            printf(" ");
        printf(" %d", i/10);

    }

    printf("\n  ");   
    for (int i = 0; i < s.dim; i++)
    {   
        int p = (i % 10) + 1;
        p != 10 ? printf(" %d", p) : printf(" 0");
    }

    printf("\n");
    printf("\033[0m");  // Tornem al color per defecte


    // Mostrem les lletres. Cada lletra ocupa dos espais: 
    // Si correspon a un encert, es marca
    for (int i = 0; i < s.dim ; i++)
    {
        printf("\033[0;31m");   // Color 
        printf("%-2d", i + 1);  // Mostrar numero de linia
        printf("\033[0m");  // Tornem al color per defecte

        for (int j = 0; j < s.dim; j++)
        {
            if (s.encertades[i * s.dim + j]) 
            {
                printf("\033[0;42m");   // Color verd de fons
                printf(" %c", s.lletres[i * s.dim + j]);
                printf("\033[0m");  // Tornem al color per defecte
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

void mostra_benvinguda(){
    printf("\n******************************************************\nUs donem la benvinguda al joc de la sopa de lletres!\nAutors: Joel, Ruben, Alberto\n******************************************************\n\n");
}

bool comprova_entrada(int n_params, char *params[]){
    bool check = false, open = false;
    FILE *exist = fopen(params[1], "r");
    if(n_params < 2){   //param 2 = ruta arxiu
        printf("\nComprova que has indicat la ruta d'un arxiu de paraules!");
    }else{
        if(exist != NULL){
            check = true;
            fclose(exist);
        }else{
            printf("\nComprova que has indicat una ruta a un arxiu correcte!");
        }      
    }
    return check;
}

//@brief funcio que comprova si la longitud de la paraula es major al maxim especificat, i retorna a l'usuari un missatge en cas d'error
bool comprova_paraula(unsigned int mida){
    bool valid = true;
    if(mida > MAX_LLETRES){
        printf("\nL'arxiu especificat conté paraules que sobrepasen el limit de lletres");
        valid = false;
    }else if(mida < MIN_LLETRES){
        printf("\nL'arxiu especificat conté paraules que no respeten el limit de lletres");
        valid = false;
    }
    return valid;
}

//@brief funcio que comprova la quantitat de paraules que hi ha a l'arxiu
int comprova_arxiu(char *route){
    char p_temp[MAX_LLETRES];       //en compte d'aquesta constant millor ficar un valor maxim per rebre qunsevol cosa...
    bool arxiuValid = true;
    unsigned int count_paraula = 0;
    FILE *t_file = fopen(route, "r");
    if(t_file != NULL){
        while (fscanf(t_file, "%s", p_temp) != EOF) {
            if(comprova_paraula(strlen(p_temp))){        //conta les paraules de l'arxiu mentres fa una cerca de possibles paraules invalides, si aixo es compleix invalida l'arxiu
                count_paraula++;
            }else{
                arxiuValid = false;
            }    
        }
        fclose(t_file);
    }
    if(!arxiuValid) count_paraula = 0;  //reinicia contador de paraules ja que invalidem tot l'arxiu 
    return count_paraula;
}



//@brief funcio que carrega les paraules de l'arxiu en un array de paraules que compleixen les regles de mida a la memoria optimes.
bool carregar_paraules(char *route, char **dades){
    unsigned int i = 0;
    char t_str[MAX_LLETRES-1];    //variable temporal a on emmagatzemare les paraules de l'arxiu
    char*p_temp;        //variable a on emmagatzemare les paraules de la variable t_var pero amb la mida correcta
    FILE *t_file = fopen(route, "r");       //com previament ja he pogut obrir l'arxiu ara no hauria de tenir cap de problema...
    while (fscanf(t_file, "%s", t_str) != EOF) {
        //p_temp = (char*) malloc(strlen(t_str) * sizeof(char*));   //anotacio: sizeof(char*) -> la mida d'un char es d'1 byte...
        dades[i] = (char*) malloc(strlen(t_str)* sizeof(char*));        //mida de cada paraula a dintre de l'array principal (bidimensional)      
        strcpy(dades[i], t_str);   //emmagatzema a l'array dades cada camp de l'arxiu, cada camp te la mida necesaria i no mes per estalviar espai
        i++;
    }
    fclose(t_file);
    return false;
}











void mescla(char **dades, int i, int m, int j){
    char *temp[MAX_LLETRES];
    int pos_left = i, pos_right = m + 1, k;
    k = i;
    // Mentre hi ha dades a les dues porcions
    while ((pos_left <= m) && (pos_right <= j)){ // Posem el menor i avancem
    //printf("\n%s",dades[pos_left]);
    //printf("\n%s",dades[pos_right]);
        if (strcmp(dades[pos_left], dades[pos_right]) < 0){     //dades[left] es mes petit que dades[right]
            temp[k] = dades[pos_left];
            pos_left++;
        }else{    //dades[left] es mes gran que dades[right] O son strings completament iguals, en aquest cas dona igual l'ordre aixi que quedara aqui
            temp[k] = dades[pos_right];             
            pos_right++;
        }
        k++;
    }
    // S'ha esgotat alguna porcio
    // Nomes copiarem el que
    //no s'hagi acabat
    while (pos_right <= j){
        temp[k] = dades[pos_right];
        pos_right++;
        k++;
    }
    while (pos_left <= m){
        temp[k] = dades[pos_left];
        pos_left++;
        k++;
    }
    // Copiem les dades mesclades
    for (k = i; k <= j; k++){
        dades[k] = (char*) malloc(strlen(temp[k])*sizeof(char*));   //com reorganitzem les paraules aquestes posicions de l'array no tenen la mida correcta
        dades[k] = temp[k];
        //printf("\n%d", k);
        //printf("\n%s", temp[k]);
    }
}







//decisio de disseny: algoritme de ordenacio mergesort, molt eficient amb grans quantitats de dades (en aquest cas no son quantitats molt grans pero haura d'iterar sobre cada lletra de les paraules)
void separa_i_mescla(char **dades, int i, int j){
    int m;
    if (j != i){
        m = (i + j)/2;      //valor a la meitat (frontera)
        separa_i_mescla(dades, i, m); // Meitat esquerra
        separa_i_mescla(dades, m + 1, j); // Meitat dreta
        mescla(dades, i, m, j);
    }
}






//arxiu per parametre, per accedir a la direcció cal treballar amb: argv[1]
//exemple d'execucio: ./main paraules.txt
int main(int argc, char *argv[]) {
    FILE *f;
    unsigned int mida_t;     //variable a on emmagatzenare la mida de la taula
    bool validate = false;
    char**dades;
    int i, j;            
    mostra_benvinguda();        //mostra missatge de benvinguda 

    //comprova si les dades d'entrada son correctes
    if(comprova_entrada(argc, argv)){        //en el cas de que la entrada sigui la esperada i l'arxiu existeixi
        mida_t = comprova_arxiu(argv[1]);
        if(mida_t != 0) validate = true;    
    }     
      
    if(validate){
        dades = malloc(mida_t*sizeof(char*));    
        carregar_paraules(argv[1], dades);
       
        separa_i_mescla(dades,i,mida_t-1);      //j = mida_t - 1 ja que volem n posicions desde 0 fins a n
        
         for(int a = 0; a < 5; a++){
            printf("\n%s",dades[a]);    //a dades hi ha les paraules de l'arxiu
        }








        free(dades);
    }
    //sopa_t sopa;    // La sopa de lletres

    //genera_sopa(&sopa);     // La generem (exemple)

   // mostra_sopa(sopa);      // La mostrem per pantalla

    return 0;
}
