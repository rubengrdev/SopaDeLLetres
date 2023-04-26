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
    bool check = false;
    FILE *exist = fopen(params[1], "r");
    if(n_params < 2){   //param 2 = ruta arxiu
        printf("\nComprova que has indicat la ruta d'un arxiu de paraules!");
    }else{
        if(exist != NULL){
            check = true;
        }else{
            printf("\nComprova que has indicat una ruta a un arxiu correcte!");
        }      
    }
    fclose(exist);
    return check;
}


bool comprova_arxiu(char *route, unsigned short *mida_t){
    bool ok = false;
    char p_temp[MAX_LLETRES];       //compte de no tallar paraules!!
    unsigned short count_paraula = 0;
    FILE *t_file = fopen(route, "r");
    while (fscanf(t_file, "%s", p_temp) != EOF) {
        count_paraula++;
    }
    fclose(t_file);
    *mida_t = count_paraula;    //emmagatzemo la quantitat de paraules que hi ha a l'arxiu
    if(count_paraula < MIN_PARAULES) printf("\nL'arxiu especificat no te suficients paraules"); ok=true;
    return ok;
}




bool carregar_paraules(char *route, char *dades[]){
    unsigned short i = 0;
    char p_temp[MAX_LLETRES];       //compte de no tallar paraules!!
    FILE *t_file = fopen(route, "r");
    while (fscanf(t_file, "%s", p_temp) != EOF) {
        dades[i] = p_temp;
        //printf("\n%s", p_temp);
        i++;
    }
    dades[i+1] = '\0';
    fclose(t_file);
    return false;
}

//arxiu per parametre, per accedir a la direcció cal treballar amb: argv[1]
//exemple d'execucio: ./main paraules.txt
int main(int argc, char *argv[]) {
    FILE *f;
    unsigned short *mida_t;     //variable a on emmagatzenare la mida de la taula on emmagatzenare el contingut de l'arxiu        
    bool validate = false;  
    char *dades[MAX_PARAULES];
    mostra_benvinguda();        //mostra missatge de benvinguda 

    if(comprova_entrada(argc, argv) && comprova_arxiu(argv[1], mida_t)){        //en el cas de que la entrada sigui la esperada i l'arxiu existeixi
        validate = true;                                                       //obrim l'arxiu i si les dades son correctes l'emmagatzenem a un array "dades"
        
        carregar_paraules(argv[1], dades);
        
        for(int a = 0; a < sizeof(dades);a++){
            printf("\n%s", dades[a]);
        }
    }     
      
    
  
    //sopa_t sopa;    // La sopa de lletres

    //genera_sopa(&sopa);     // La generem (exemple)

   // mostra_sopa(sopa);      // La mostrem per pantalla

    return 0;
}
