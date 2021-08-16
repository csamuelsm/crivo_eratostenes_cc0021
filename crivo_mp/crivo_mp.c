#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "omp.h"

int CrivoDeEratostenes(int* vetor, long int n, int printaPrimos) {
    
    int count_primos = 0;

    // criando um vetor booleano para valores de 0 ate n
    // e setando todos os valores para true
    // este vetor ira representar os valores que sao primos (true) ou nao (false)
    //#pragma omp for
    for (int i = 0; i <= n; i++) {
        vetor[i] = 1;
    }

    #pragma omp parellel for num_threads(4)
    { 
    for (int p = 2; p*p <= n; p++) {
        if (vetor[p] == 1) {
            // "remover" opcoes de primos
            for (int i = p*p; i <= n; i+=p) {
                vetor[i] = 0;
            }
        }
    }
    }
        
    #pragma omp parellel for reduction(+: count_primos) num_threads(4)
    {
    for (int i = 2; i <= n; i++) {
        if (vetor[i] == 1) {
            if (printaPrimos == 1){
                printf("%d\t", i);
            }
        count_primos++;
        }
    }
    }

    return count_primos;
}

int main()
{
    long int n = 10000000;
    //printf("Digite a quantidade de numeros a ser verificada: ");
    //scanf("%ld", &n);
    clock_t t1 = clock();
    int* primos = (int*) malloc(n * sizeof(int));
    int n_primos = CrivoDeEratostenes(primos, n, 0);
    printf("\nQuantidade de primos ate %ld: %d\n", n, n_primos);
    free(primos);
    clock_t t2 = clock();
    printf("Tempo necessário para rodar: %lf\n", (double)(t2-t1)/CLOCKS_PER_SEC);
    return 0;
}
