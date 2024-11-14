// assumption: all matrices are square

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX_NUM_THREADS 10
int MAX;
int **matA; 
int **matB; 
int **matSumResult;
int **matDiffResult; 
int **matProductResult; 

struct matrixCellAddr {
  int row, col;
};

void fillMatrix(int** matrix) {
    /*
    fillMatrix takes in a matrix input and fills its cells with random values
    parameters:
        matrix (int **): a matrix whose values will be randomly filled in by the function
    returns:
        void
    */
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            matrix[i][j] = rand()%10+1;
        }
    }
}

void printMatrix(int** matrix) {
    /*
    printMatrix takes in a matrix input and prints its values as a grid
    parameters:
        matrix (int **): a matrix whose values will be printed by the function
    returns:
        void
    */
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            printf("%5d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}


void* computeSumCell(void* args) { 
    /*
    computeSumCell fetches the appropriate coordinates from the argument, and sets the cell of matSumResult at the coordinates to the sum of the values at the coordinates of matA and matB.
    parameters:
        void* args: this is a void pointer which expects a matrixCellAddr struct input with the row and column of the target cell in matSumResult
    */

    struct matrixCellAddr * targetCellAddr = (struct matrixCellAddr*) (args);
    matSumResult[targetCellAddr->row][targetCellAddr->col] = matA[targetCellAddr->row][targetCellAddr->col]+matB[targetCellAddr->row][targetCellAddr->col];
    return NULL;
}


void* computeDiffCell(void* args) { 
    /*
    computeDiffCellFetches the appropriate coordinates from the argument, and sets the cell of matSumResult at the coordinates to the difference of the values at the coordinates of matA and matB.\
    parameters:
        void* args: this is a void pointer which expects a matrixCellAddr struct input with the row and column of the target cell in matDiffesult
    */
    struct matrixCellAddr * targetCellAddr = (struct matrixCellAddr*) (args);
    matDiffResult[targetCellAddr->row][targetCellAddr->col] = matA[targetCellAddr->row][targetCellAddr->col]-matB[targetCellAddr->row][targetCellAddr->col];
    return NULL;
}


void* computeProductCell(void* args) { 
    /*
    computeProductCell fetches the appropriate coordinates from the argument, and sets the cell of matSumResult at the coordinates to the inner product of matA and matB.
    parameters:
        void* args: this is a void pointer which expects a matrixCellAddr struct input with the row and column of the target cell in matProductResult
    */
    struct matrixCellAddr * targetCellAddr = (struct matrixCellAddr*) (args);
    int i, product;
    product = 0;
    for (i=0;i<MAX;i++){
        product = product + (matA[targetCellAddr->row][i]*matB[i][targetCellAddr->col]);
    }
    matProductResult[targetCellAddr->row][targetCellAddr->col] = product;
    return NULL;
}

void computeFuncMatrix(void *(*func)(void *)){
    /*
    computeFuncMatrix does matrix computations in parallel using threads
    parameters:
        func (void *(*func)(void *)): a function pointer that returns a void pointer. function is expected to take in an argument of a targetCellAddr address
    returns:
        NULL
    */

    // INSTRUCTIONS:
    // 3. Create pthread_t objects for our threads.
    // 4. Create a thread for each cell of each matrix operation.
    // 5. Wait for all threads to finish.
    
    // TODO: get threads to be max 10 (MAX_NUM_THREADS) and test
    int target_cell_indx, row, col, thread_indx, threads_used;
    pthread_t threads[MAX_NUM_THREADS]; // MAX*MAX array of threads since there are MAX*MAX cells per matrix
    struct matrixCellAddr targetCellAddrs[MAX*MAX]; // holds target cells separately in different indices of array so that unexpected things don't occur, since threads share same memory
    threads_used = MAX_NUM_THREADS; // dealing with edge case where matrix n*n < max number of threads
    if ((MAX*MAX)<MAX_NUM_THREADS){
        threads_used=MAX*MAX;
    }

    target_cell_indx=0;
    thread_indx=0;
    for (row=0; row<MAX; row++){
        for (col=0; col<MAX; col++){
            printf("target_cell_indx: %d\n",target_cell_indx);
            targetCellAddrs[target_cell_indx].row = row; // put the row into targetCellAddr[target_cell_indx] struct
            targetCellAddrs[target_cell_indx].col = col;
            pthread_create(&threads[thread_indx], NULL, func, &targetCellAddrs[target_cell_indx]); // pass targetCellAddrs[i] into given func such as computeSumCell to carry out the computation (sum) on the cell
            thread_indx++;
            printf("1 thread_indx: %d\n", thread_indx);
            target_cell_indx ++;
            if (thread_indx==threads_used){
                
                for (thread_indx=0; thread_indx<threads_used; thread_indx++){
                    printf("%d threads_used; thread_indx: %d\n", threads_used, thread_indx);
                    pthread_join(threads[thread_indx], NULL); // wait for thread to finish
                }
                thread_indx=0;
                
            }
            
        }
    }

    for (thread_indx=0; thread_indx<threads_used; thread_indx++){
        pthread_join(threads[thread_indx], NULL); // wait for thread to finish
    }
}


int main(int argc, char *argv[]) {
    int i, row, col;
    srand(time(0));  // Do Not Remove. Just ignore and continue below.
    
    // 0. Get the matrix size from the command line and assign it to MAX
    if (argc<2){
        fprintf(stderr, "Pass in the size of the matrix through command line\n");
        return 0;
    }
    else{
        MAX = atoi(argv[1]);
    }

    // initialize all matrices to size n*n where n = MAX
    matA = malloc(MAX * sizeof *matA);
    matB = malloc(MAX * sizeof *matB);
    matSumResult = malloc(MAX * sizeof *matSumResult);
    matDiffResult = malloc(MAX * sizeof *matDiffResult);
    matProductResult = malloc(MAX * sizeof *matProductResult);
    for (i=0; i<MAX; i++)
    {
        matA[i] = malloc(MAX * sizeof *matA[i]);
        matB[i] = malloc(MAX * sizeof *matB[i]);
        matSumResult[i] = malloc(MAX * sizeof *matSumResult[i]);
        matProductResult[i] = malloc(MAX * sizeof *matProductResult[i]);
        matDiffResult[i] = malloc(MAX * sizeof *matDiffResult[i]);
    }
        
    // 1. Fill the matrices (matA and matB) with random values.
    fillMatrix(matA);
    fillMatrix(matB);
    
    // 2. Print the initial matrices.
    printf("Matrix A:\n");
    printMatrix(matA);
    printf("Matrix B:\n");
    printMatrix(matB);
    
    // computeFuncMatrix takes on responsibility of creating threads for each cell of the matrix and does the computation for the target matrix based on the function you give it
    computeFuncMatrix(computeSumCell); // gets sum of matA and matB, result stored in matSumResult
    computeFuncMatrix(computeDiffCell); // gets difference of matA and matB, result stored in matDiffResult
    computeFuncMatrix(computeProductCell); // gets dot product of matA and matB, result stored in matProductResult
    
    
    // 6. Print the results.
    printf("Results:\n");
    printf("Sum:\n");
    printMatrix(matSumResult);
    printf("Difference:\n");
    printMatrix(matDiffResult);
    printf("Product:\n");
    printMatrix(matProductResult);

    // free memory used to dynamically allocate matrices
    for (i=0; i<MAX; i++)
    {
        free(matA[i]);
        free(matB[i]);
        free(matSumResult[i]);
        free(matDiffResult[i]);
        free(matProductResult[i]);
    }
    free(matA);
    free(matB);
    free(matSumResult);
    free(matDiffResult);
    free(matProductResult);
    return 0;
  
}