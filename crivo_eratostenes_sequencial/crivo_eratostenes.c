#include <stdio.h>
#include <stdlib.h>


int CrivoDeEratostenes(int n, int printaPrimos) {
    // criando um vetor booleano para valores de 0 at� n
    // e setando todos os valores para true
    // este vetor ir� representar os valores que s�o primos (true) ou n�o (false)
    int primos[n+1];
    for (int i = 0; i <= n; i++) {
        primos[i] = 1;
    }

    for (int p = 2; p*p <= n; p++) {
        if (primos[p] == 1) {
            // "remover" op��es de primos
            for (int i = p*p; i <= n; i+=p) {
                primos[i] = 0;
            }
        }
    }

    int count_primos = 0;
    for (int i = 2; i <= n; i++) {
        if (primos[i] == 1) {
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
    int primos = CrivoDeEratostenes(10000000, 0);
    printf("\nQuantidade de primos ate 10.000.000: %d\n", primos);
    return 0;
}
