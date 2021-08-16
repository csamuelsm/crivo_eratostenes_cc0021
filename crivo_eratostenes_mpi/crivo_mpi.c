#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TAM 10000000
//#define ELEMS_PER_PROC 5000000  // aqui é para 2 cores (5000000 * 2) = 10000000

int main(int argc, char** argv) {

    MPI_Init(NULL, NULL);

    // Pegando alguns dados iniciais do MPI antes de começar
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    double t1, t2;

    if (world_rank == 0) {
    	t1 = MPI_Wtime();
    }

    int ELEMS_PER_PROC = ceil(TAM/world_size);
    //int TAM = ELEMS_PER_PROC * world_size;
    int sqrtTAM = floor(sqrt(TAM));

    int * primos_bloco = malloc(sizeof(int)*(ELEMS_PER_PROC));
    int * primos = (int *)malloc(sizeof(int)*(TAM+1));

    // Iniciando vetor booleano
    if (world_rank == 0) {

        for (int i = 0; i <= TAM; i++) {
            primos[i] = 1;
        }

        primos[0] = 0;
        primos[1] = 0; // 0 e 1 não são primos

        // Vamos precisar primeiro dos números primos de 2 ate sqrt(TAM)
        // pois podemos encontrar todos os outros de sqrt(TAM) ate TAM somente com eles
        for (int p = 2; p*p <= sqrtTAM; p++) {
            if (primos[p] == 1) {

                for (int i = p*p; i <= TAM; i+=p) {
                    primos[i] = 0;
                }
                
            }
        }

    }

    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Bcast(&primos[0], sqrtTAM+1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(primos, ELEMS_PER_PROC, MPI_INT, &primos_bloco[0], ELEMS_PER_PROC, MPI_INT, 0, MPI_COMM_WORLD);

    int primeiro = world_rank*ELEMS_PER_PROC + 1;
    //printf("RANK %d: PRIMEIRO = %d\n", world_rank, primeiro);

    for (int p = 2; p <= sqrtTAM; p++) {
        if (primos[p] == 1) {
            int n = primeiro;
            n = n + p/2;
            n = n - (n%p);
            if (n < primeiro) {
                n = n + p;
            }
            if (n == p) {
                n = n + p;
            }

            for (int i = n-primeiro+1; i < ELEMS_PER_PROC; i+=p) {
                //printf("%d\t", i);
                primos_bloco[i] = 0;
            }
        }
    }

    int sub_primos = 0;

    for (int i = 2; i < ELEMS_PER_PROC; i++) {
        if (primos_bloco[i] == 1) {
            sub_primos++;
        }
    }

    printf("\nRANK %d: %d primos encontrados de %d a %d\n\n", world_rank, sub_primos, primeiro, primeiro+ELEMS_PER_PROC-1);
    /*for (int i = 0; i < ELEMS_PER_PROC; i++) {
        printf("RANK %d: [%d: %d]\n", world_rank, i+primeiro-1, primos_bloco[i]);
    }*/

    int * primos_total = NULL;
    if (world_rank == 0) {
        primos_total = malloc(sizeof(int) * world_size);
    }

    MPI_Gather(&sub_primos, 1, MPI_INT, primos_total, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (world_rank == 0) {
        int total = 0;
        for (int i = 0; i < world_size; i++) {
            total += primos_total[i];
            //printf("%d\t", primos_total[i]);
        }
        printf("Total primos: %d\n", total);

	t2 = MPI_Wtime();

    	printf("Tempo necessário para rodar: %lf\n", t2-t1);
    }

    MPI_Finalize();

    free(primos);

    return 0;

}