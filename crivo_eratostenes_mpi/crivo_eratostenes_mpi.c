#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ELEMS_PER_PROC 5000000 // aqui é para 2 cores (5000000 * 2) = 10000000

int main(int argc, char** argv) {

    MPI_Init(NULL, NULL);

    // Pegando alguns dados iniciais do MPI antes de começar
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int TAM = ELEMS_PER_PROC * world_size;
    int sqrtTAM = floor(sqrt(TAM));

    int * primos_bloco = (int *)malloc(sizeof(int)*(ELEMS_PER_PROC));
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

        // Enviando as respectivas partes para cada processo
        for (int processos = 1; processos < world_size; processos++) {
            MPI_Send(&primos[0], sqrtTAM+1, MPI_INT, processos, 0, MPI_COMM_WORLD);
            printf("RANK %d: Enviado vetor primo do 0 ate %d para processo %d\n", world_rank, sqrtTAM+1, processos);
            int idx = (processos*ELEMS_PER_PROC) + 1;
            primos_bloco = &primos[idx];
            MPI_Send(&primos_bloco[0], ELEMS_PER_PROC, MPI_INT, processos, 0, MPI_COMM_WORLD);
            printf("RANK %d: Enviado bloco P%d do %d ate %d para processo %d\n", world_rank, processos, idx, idx+ELEMS_PER_PROC, processos);
        }

        // Crivando sua parte
        for (int p = 2; p <= sqrtTAM; p++) {

            if (primos[p] == 1) {

                int inc = 1;
                for (int i = sqrtTAM+1; i <= TAM; i+=inc) {
                    if (i % p == 0) {
                        inc = primos[p];
                        primos[i] = 0;
                    }
                }

            }

        }

        printf("RANK %d: Crivo feito\n", world_rank);

        int count = 0;
        for (int i = 0; i <= ELEMS_PER_PROC; i++) {
            if (primos[i] == 1) {
                count++;
            }
        }

        printf("RANK %d: Quantidade de primos encontrados: %d\n", world_rank, count);

        int total_primos = count;

        for (int processos = 1; processos < world_size; processos++) {

            MPI_Recv(&count, 1, MPI_INT, processos, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("RANK %d: Recebido contador de processo %d\n", world_rank, processos);

            total_primos += count;

        }

        printf("Quantidade total de primos encontrada até %d: %d\n", TAM, total_primos);

    } else {
        MPI_Recv(&primos[0], sqrtTAM+1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("RANK %d: Recebido vetor primo do processo 0\n", world_rank);
        MPI_Recv(&primos_bloco[0], ELEMS_PER_PROC, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("RANK %d: Recebido bloco do processo 0\n", world_rank);

        int primeiro = world_rank*ELEMS_PER_PROC + 1; // esse é o primeiro número do bloco

        // crivando sua parte
        for (int p = 2; p <= sqrtTAM; p++) {

            if (primos[p] == 1) {

                int inc = 1;

                for (int i = 0; i <= ELEMS_PER_PROC; i+= inc) {

                    if ((i + primeiro) % p == 0) {
                        primos_bloco[i] = 0;
                        inc = primos[p];
                    } 

                }

            }

        }

        printf("RANK %d: Crivo feito\n", world_rank);

        int count = 0;
        for (int i = 0; i <= ELEMS_PER_PROC; i++) {
            if (primos_bloco[i] == 1) {
                count++;
            }
        }

        printf("RANK %d: Quantidade de primos encontrados: %d\n", world_rank, count);

        MPI_Send(&count, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        printf("RANK %d: Contador de primos enviado para processo 0\n", world_rank);
    }

    printf("Finalizando...\n");

    MPI_Finalize();

    return 0;

}