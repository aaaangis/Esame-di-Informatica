#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VEICOLI     100
#define MAX_LEN_TARGA   10
#define MAX_LEN_CID     10
#define MAX_LEN_STRING  50

typedef struct s_elem_garage{
    char targa[MAX_LEN_TARGA];
    char cid[MAX_LEN_CID];
    float costo;
} t_elem_garage;

typedef struct s_elem_registro{
    char cid[MAX_LEN_CID];
    char nome[MAX_LEN_STRING];
    char cognome[MAX_LEN_STRING];
    char anno_nascita[5]; // AAAA\0
} t_elem_registro;


void print_menu()
{
    printf("Ecco cosa puoi fare:\n\n");
    printf("1) Leggi il file garage:\n");
    printf("2) Leggi il file registro:\n");
    printf("3) Stampa l'anagrafica di chi ha registrato la carta di identita':\n");
    printf("4) Stampa l'anagrafica dei clienti, ordinati dal piu' anziano al piu' giovane\n");
    printf("5) Stampa la targa dell'auto che ha speso di piu' per il parcheggio\n");
    printf("6) Stampa i clienti che hanno speso piu' di una certa soglia\n");
    printf("7) Stampa i clienti che hanno piu' di una macchina\n");
    printf("8) Determina l'importo medio speso dai clienti\n");
    printf("9) Scrivi su un file in output il resoconto cliente - importo dovuto\n");
    printf("0) ESCI\n");
    printf(">> ");

}

void print_targhe_da_cid(const char* cid , t_elem_garage *garage)
{
    /* stampa le auto di un proprietario identificato tramite cid*/
    int i = 0, p = 0;
    for(i = 0; i < MAX_VEICOLI; i++)
    {
        if (strcmp(cid, garage[i].cid) == 0)
        {
            printf("%d) %s\n", i+1, garage[i].targa);
            p++;
        }    
    }

    if(p == 0) printf("Nessuna targa registrata a %s", cid);
}

void print_anagrafica_registro(t_elem_registro elem_registro){
    /* Questa funzione stampa l'anagrafica di un elemento del registro */
    printf("\n");
    printf("Carta di identita' n. %s\nNome %s\nCognome %s\nAnno di nascita %s\n",
            elem_registro.cid, elem_registro.nome, elem_registro.cognome, elem_registro.anno_nascita);
}

void print_anagrafica_registro_da_cid(const char* cid, const t_elem_registro* registro)
{
    /* Questa funzione stampa l'anagrafica di un certo cliente data la carta di identita' */
    /* Prende in ingresso una cid e il puntatore all'array registro*/

    int i;
    for (i = 0; i < MAX_VEICOLI; i++)
    {
        if(strcmp(registro[i].cid, cid) == 0)
        {
            print_anagrafica_registro(registro[i]);
            return;
        }
    }
}



void scambia_elem_registro(t_elem_registro* ptr_elem1, t_elem_registro* ptr_elem2)
{
    /* Questa funzione scambia due elementi registro nell'array registro*/
    /* Serve ad implementare l'ordinamento del registro */

    t_elem_registro temp;

    temp = *ptr_elem1;
    *ptr_elem1 = *ptr_elem2;
    *ptr_elem2 = temp; 
}

void ordina_registro(t_elem_registro* registro, int dim_array)
{   
    /* Questa funzione ordina il registro in per anno di nascita crescente*/
    /* Selection sort*/
    int i, j, min_idx;
 
    // Per ogni posizione cerco in quelle successive il più piccolo
    // Man mano l'array inizia ad ordinarsi da sinistra
    for (i = 0; i < dim_array - 1; i++) {
 
        // Trova l'elemento più piccolo nel sottoarray
        min_idx = i;
        for (j = i + 1; j < dim_array; j++)
        {
            if( atoi(registro[j].anno_nascita) < atoi(registro[min_idx].anno_nascita) )
            {
                min_idx = j;
            }
        }
        // Metto l'elemento più piccolo al suo posto
        scambia_elem_registro(&registro[min_idx], &registro[i]);
    }
}


int main(void){

    unsigned int scelta = 0;
    unsigned int n_garage_elem = 0, n_registro_elem = 0; // Tengo conto se sono stati letti i file
    int i,j, n_macchine;
    float max_spesa, costo_totale;
    char nome_file_temp[MAX_LEN_STRING];
    FILE *f = NULL;

    char targa[MAX_LEN_TARGA];
    char cid[MAX_LEN_CID];
    float costo;
    char nome[MAX_LEN_STRING];
    char cognome[MAX_LEN_STRING];
    char anno_nascita[11]; // GG-MM-AAAA\0
    
    t_elem_garage garage[MAX_VEICOLI];
    t_elem_registro registro[MAX_VEICOLI];

    printf("Benvenuto nella gestione del garage!\n");

    do {
        print_menu();

        scanf("%u", &scelta);
        if (scelta > 9) break; // scelta invalida, esco

        switch (scelta)
        {
        case 1:
            printf("[*] Lettura file garage\n");
    
            printf("Inserisci il nome del file garage\n");
            printf(">> ");
            scanf("%s", nome_file_temp);
            
            f = fopen(nome_file_temp, "r");
            if (f == NULL)
            {
                printf("Apertura file garage fallita\n");
                break;
            }
            for ( i = 0; i < MAX_VEICOLI; i++)
            {
                if (fscanf(f, "%s %s %f", targa, cid, &costo) == EOF) break; //Leggo fino alla fine del file
                
                // Scrivo sull'elemento le informazioni
                strcpy(garage[i].targa, targa);
                strcpy(garage[i].cid, cid);
                garage[i].costo = costo;
                n_garage_elem++;
            }

            printf("Letti %d elementi\n", n_garage_elem );
            fclose(f);
            break;

        case 2:
            printf("[*] Lettura file registro\n");
            printf("Inserisci il nome del file registro\n");
            printf(">> ");
            scanf("%s", nome_file_temp);
            
            f = fopen(nome_file_temp, "r");
            if (f == NULL)
            {
                printf("Apertura file registro fallita\n");
                break;
            }

            for ( i = 0; i < MAX_VEICOLI; i++)
            {
                if (fscanf(f, "%s %s %s %s", cid, nome, cognome, anno_nascita) == EOF) break; //Leggo fino alla fine del file
                
                // Scrivo sull'elemento le informazioni
                strcpy(registro[i].cid, cid);
                strcpy(registro[i].nome, nome);
                strcpy(registro[i].cognome, cognome);
                strcpy(registro[i].anno_nascita, anno_nascita);
                n_registro_elem++;

            }

            printf("Letti %d elementi\n", n_registro_elem );
            fclose(f);
            
            break;
        case 3:
            if( n_registro_elem == 0 || n_garage_elem == 0)
            {
                printf("Devi prima leggere lo stato dai file!\n\n");
                break;
            }
            
            printf("[*] Anagrafica di chi ha registrato la carta di identita':\n");
            
            // Per ogni elemento del registro stampo le sue info
            // E stampo le macchine registrate a suo nome
            for(i = 0; i < n_registro_elem; i++)
            {
                print_anagrafica_registro(registro[i]);
                printf("Auto:\n");
                print_targhe_da_cid(registro[i].cid, garage);
                printf("\n\n");
            }
            break;
        case 4:
            if( n_registro_elem == 0 || n_garage_elem == 0)
            {
                printf("Devi prima leggere lo stato dai file!\n\n");
                break;
            }
            
            printf("[*] Anagrafica dei clienti ordinati dal piu' anziano al piu' giovane:\n");

            ordina_registro(registro, n_registro_elem);
            
            // Per ogni elemento del registro stampo le sue info
            // E stampo le macchine registrate a suo nome
            for(i = 0; i < n_registro_elem; i++)
            {
                print_anagrafica_registro(registro[i]);
                printf("Auto:\n");
                print_targhe_da_cid(registro[i].cid, garage);
                printf("\n\n");
            }

            break;
        case 5:
           if( n_registro_elem == 0 || n_garage_elem == 0)
            {
                printf("Devi prima leggere lo stato dai file!\n\n");
                break;
            }
            
            printf("[*] Stampa la targa dell'auto che ha speso di piu' per il parcheggio:\n");
            int max_spesa_idx = 0;
            for (i = 0; i < n_garage_elem; i++)
            {
                // Cerco l'indice di quello che ha speso di più
                if (garage[i].costo > max_spesa  )
                {
                    max_spesa = garage[i].costo;
                    max_spesa_idx = i;
                }
            }

            // Scorro il registro cercando la CID dello spendaccione
            printf("La targa richiesta e' %s, il proprietario è:\n", garage[max_spesa_idx].targa);

            print_anagrafica_registro_da_cid(garage[max_spesa_idx].cid, registro);

            break;
        case 6:
            if( n_registro_elem == 0 || n_garage_elem == 0)
            {
                printf("Devi prima leggere lo stato dai file!\n\n");
                break;
            }
            
            float soglia;
            printf("[*] Stampo i clienti che hanno speso di piu' di una certa soglia:\n");

            printf("Inserisci la soglia\n");
            printf(">> ");
            scanf("%f", &soglia);

            for(i = 0; i < n_garage_elem; i++)
            {
                if (garage[i].costo > soglia)
                {
                    print_anagrafica_registro_da_cid(garage[i].cid, registro);
                }
            }
            break;
        case 7:
            printf("[*] Stampo i clienti che hanno piu' di una macchina:\n");
            for(i = 0; i < n_registro_elem; i++)
            {
                // Per ogni cliente, controllo se ha più di una macchina
                n_macchine = 0;
                for (j = 0; j < n_garage_elem; j++)
                {
                    if( strcmp(garage[j].cid, registro[i].cid) == 0){
                        n_macchine++;
                    }
                }
                if(n_macchine > 1) print_anagrafica_registro(registro[i]);
            }
            break;
        case 8:
            printf("[*] Calcolo l'importo medio speso dai clienti\n");
            costo_totale = 0;
            for (i = 0; i < n_garage_elem; i++)
            {
                costo_totale += garage[i].costo;
            }

            printf("L'importo medio è %.2f\n", costo_totale/n_garage_elem);
            break;
        case 9:
            if( n_registro_elem == 0 || n_garage_elem == 0)
            {
                printf("Devi prima leggere lo stato dai file!\n\n");
                break;
            }
            printf("[*] Calcolo il resoconto cliente - importo dovuto \n");
            
            printf("[*] Scrittura file resoconto\n");
            printf("Inserisci il nome del file resoconto\n");
            printf(">> ");
            scanf("%s", nome_file_temp);
            
            f = fopen(nome_file_temp, "w");
            if (f == NULL)
            {
                printf("Apertura file resoconto fallita\n");
                break;
            }

            /* Per ogni persona, controllo l'importo dovuto e lo stampo*/
            fprintf(f, "CLIENTE - IMPORTO DOVUTO\n\n");

            for(i = 0; i < n_registro_elem; i++)
            {
                costo = 0; //inizializzo l'importo dovuto da ogni cliente
                for( j = 0; j < n_garage_elem; j++)
                {
                    if(strcmp(garage[j].cid, registro[i].cid) == 0)
                    {
                        // dalla carta di identità controllo che l'entry del garage sia di quel cliente specifico
                        costo += garage[j].costo;
                    }
                }
                fprintf(f, "%s %s %s %s %.2f\n", registro[i].cid, registro[i].nome, registro[i].cognome, registro[i].anno_nascita, costo);
            }

            printf("File scritto correttamente!\n");
            fclose(f);
            
            break;
        }

    } while (scelta != 0); // Ripeto finchè non si vuole uscire

    printf("Ciao!\n");
}
