#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#define A 6
#define N 6     //tamanho da matriz resultante: NxN
#define B 6

void printMatrix(int a[N][N]);

void main(int argc, char **argv){
    int i, j, k=0, myRank, np, from, to, matrixR[N][N],
    matrixA[A][N] = {   {1,2,3,4,5,6},
                        {7,8,9,8,7,6},
                        {1,2,3,4,5,6},
                        {7,8,9,8,7,6},
                        {1,2,3,4,5,6},
                        {7,8,9,8,7,6}   },
    matrixB[N][B] = {   {1,2,3,4,5,6},
                        {7,8,9,8,7,6},
                        {1,2,3,4,5,6},
                        {7,8,9,8,7,6},
                        {1,2,3,4,5,6},
                        {7,8,9,8,7,6}   };

	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	MPI_Comm_size(MPI_COMM_WORLD, &np);

	if ((N%np) != 0){
        if (myRank == 0) printf("\n ** ERRO: Tamanho da matriz nao divisivel pelo numero de processos. "
                                "Use um numero que %d seja divisivel por ele. **\n\n", N);
        MPI_Finalize();
        exit(EXIT_FAILURE);
	}

    from = myRank * N/np;
    to = (myRank+1) * N/np;

    MPI_Bcast(matrixB, N*N, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(matrixA, N*N/np, MPI_INT, matrixA[from], N*N/np, MPI_INT, 0, MPI_COMM_WORLD);

    printf("Processo %d, calculando da linha %d a %d.\n", myRank, from, to-1);
	for (i=from; i<to; i++)
            for(j=0 ; j<N ; j++){
                matrixR[i][j]=0;
                for(k=0; k<N; k++) matrixR[i][j] += (matrixA[i][k]*matrixB[k][j]);
            }

    MPI_Gather(matrixR[from], N*N/np, MPI_INT, matrixR, N*N/np, MPI_INT, 0, MPI_COMM_WORLD);

    if(myRank == 0) printMatrix(matrixR);
    MPI_Finalize();
}

void printMatrix(int a[N][N]){
    int i, j;
    printf("\nEssa e a matriz produto: ");
    for (i=0; i<N ; i++){
        printf("\n  ");
        for (j=0; j<N ; j++) printf("%d\t",a[i][j]);
    }
    printf("\n");
}

// Referências bibliográficas:
// * http://willianjusten.com.br/
// * http://mpitutorial.com/
