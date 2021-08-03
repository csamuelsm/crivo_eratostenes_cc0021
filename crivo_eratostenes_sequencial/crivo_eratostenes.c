#include <stdio.h>
#include <stdlib.h>


int CrivoDeEratostenes(int* vetor, int n, int printaPrimos) {
    // criando um vetor booleano para valores de 0 at� n
    // e setando todos os valores para true
    // este vetor ir� representar os valores que s�o primos (true) ou n�o (false)
    for (int i = 0; i <= n; i++) {
        vetor[i] = 1;
    }

    for (int p = 2; p*p <= n; p++) {
        if (vetor[p] == 1) {
            // "remover" op��es de primos
            for (int i = p*p; i <= n; i+=p) {
                vetor[i] = 0;
            }
        }
    }

    int count_primos = 0;
    for (int i = 2; i <= n; i++) {
        if (vetor[i] == 1) {
            if (printaPrimos == 1){
                printf("%d\t", i);
            }
            count_primos++;
        }
    }

    return count_primos;
}

int main()
{
    long int n;
    scanf("%ld", &n);
    int* primos = (int*) malloc(n * sizeof(int));
    int n_primos = CrivoDeEratostenes(primos, n, 0);
    printf("\nQuantidade de primos ate %ld: %d\n", n, n_primos);
    return 0;
}
