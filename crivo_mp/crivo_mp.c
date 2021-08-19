#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "omp.h"

/*
int CrivoDeEratostenes(int* vetor, long int n, int printaPrimos) {
    
    return count_primos;
}
*/

int main()
{
    long int n = 10000000;
    int printaPrimos = 0;
    //printf("Digite a quantidade de numeros a ser verificada: ");
    //scanf("%ld", &n);
    clock_t t1 = clock();
    int* primos = (int*) malloc(n * sizeof(int));
    //int n_primos = CrivoDeEratostenes(primos, n, 0);
    
    int count_primos = 0;
    long int raiz = sqrt(n);

    // criando um vetor booleano para valores de 0 ate n
    // e setando todos os valores para true
    // este vetor ira representar os valores que sao primos (true) ou nao (false)
    
    #pragma omp parellel num_threads(2)
    {
    #pragma omp parallel for
    for (int i = 0; i <= n; i++) {
        primos[i] = 1;
    }

    #pragma omp parellel for schedule(dynamic)
    for (int p = 2; p <= raiz; p++) {
        if (primos[p] == 1) {
            // "remover" opcoes de primos
            for (int i = p*p; i <= n; i+=p) {
                primos[i] = 0;
            }
        }
    }
        
    #pragma omp parallel for reduction(+: count_primos)
    for (int i = 2; i <= n; i++) {
        if (primos[i] == 1) {
            if (printaPrimos == 1){
                printf("%d\t", i);
            }
        count_primos++;
        }
    }

    }
    
    printf("\nQuantidade de primos ate %ld: %d\n", n, count_primos);
    free(primos);
    clock_t t2 = clock();
    printf("Tempo necessário para rodar: %lf\n", (double)(t2-t1)/CLOCKS_PER_SEC);

    FILE *out_file = fopen("desempenho.txt", "a"); 
    fprintf(out_file, "%lf\n", (double)(t2-t1)/CLOCKS_PER_SEC);
    fclose(out_file);

    return 0;
}
