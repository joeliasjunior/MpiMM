#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#define A 6
#define N 6     //tamanho da matriz resultante: NxN
#define B 6

void printMatrix(int a[N][N]);

void main(int argc, char **argv){
    int i, j, k=0, myRank, tag = 1, target = 0, np, from, to, matrixR[N][N],
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

	if (np != N){
        if (myRank == 0) printf("\n ** ERRO: Tamanho da matriz nao compativel com numero de processos. "
                                "Use \"np %d\" para compilar. **\n\n", N);
        MPI_Finalize();
        exit(EXIT_FAILURE);
	}

    from = myRank * N/np;
    to = (myRank+1) * N/np;

    for (i=from; i<to; i++)
            for(j=0 ; j<N ; j++){
                matrixR[i][j]=0;
                for(k=0; k<N; k++) matrixR[i][j] += (matrixA[i][k]*matrixB[k][j]);
            }

    if(myRank != 0)
        for(i=from; i<to; i=i+np) MPI_Send(&matrixR[i][0], N, MPI_INT, target, tag, MPI_COMM_WORLD);

    if(myRank == 0)
        for(j = 1; j<np; j++)
            for(i=j ; i<N ; i=i+np) MPI_Recv(&matrixR[i][0], N, MPI_INT, j, tag, MPI_COMM_WORLD, &status);

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
