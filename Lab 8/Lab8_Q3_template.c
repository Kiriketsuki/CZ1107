#include <stdio.h>
#include <stdlib.h>

int nQueens(int** board, int N, int col);
int isSafe(int** board,int N, int row, int col);
void printSolution(int** board, int N);

int main()
{
    int **board;
    int BSize;

    int i,j;

    printf("Enter the size of chessBoard:\n");
    scanf("%d",&BSize);

    board = (int **)malloc(BSize*sizeof(int *));
    for(i=0;i<BSize;i++)
        board[i] = (int *)malloc(BSize*sizeof(int));

    for(i=0;i<BSize;i++)
       for(j=0;j<BSize;j++)
           board[i][j] = 0;

    nQueens(board, BSize, 0);
    printSolution(board, BSize);
    return 0;
}

int nQueens(int** board, int BSize, int col) // col is the current column you are trying to place the queen in
{
    int i;
    // printf("b_size is %d\n", BSize);

    if (col >= BSize) { // if placing outside of range, all queens are placedd, solution has been found, can print
        printSolution(board, BSize);
        // for (i = 0; i < BSize; i++) {
        //     for (int j = 0; j < BSize; j++) {
        //         board[i][j] = 0;
        //     }
        // }
        return 1;
    }
    int result = 0;

    for (i = 0; i < BSize; i++) {
        // printf("i is %d\n", i);
        if (isSafe(board, BSize, i, col)) { // can place
            board[i][col] = 1;
            result = result || nQueens(board, BSize, col+1); // start recursion
            board[i][col] = 0; // clean board for backtracking
        }
    }
    return result; // cannot be placed in any row in this column, time to back track
}

//Safe checking
int isSafe(int** board,int BSize, int row, int col)

{
    int i, j;

    // Horicatal check
    for (i = 0; i < col; i++)
        if (board[row][i])
            return 0;

    // Upper left diagonal check
    for (i = row, j = col; i >= 0 && j >= 0; i--, j--)
        if (board[i][j])
            return 0;

    // Lower left diagonal check
    for (i = row, j = col; j >= 0 && i < BSize; i++, j--)
        if (board[i][j])
            return 0;

    return 1;
}

void printSolution(int** board, int BSize)
{
    int i,j;
    for (i = 0; i < BSize; i++) {
        for (j = 0; j < BSize; j++){
            if(board[i][j]==1)
                printf(" Q ");
            else
                printf(" + ");
        }
        printf("\n");
    }
    printf("\n");
}
