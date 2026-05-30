#include <stdio.h>
#include "canvas.h"

char grid[ROWS][COLS];

void setupGrid()
{
    int i, j;
    for(i = 0; i < ROWS; i++)
    {
        for(j = 0; j < COLS; j++)
        {
            grid[i][j] = ' ';
        }
    }
}

void resetGrid()
{
    setupGrid();
}

int checkX(int x)
{
    if(x >= 0 && x < COLS)
        return 1;
    return 0;
}

int checkY(int y)
{
    if(y >= 0 && y < ROWS)
        return 1;
    return 0;
}

void showGrid()
{
    int i, j;
    printf("+");
    for(j = 0; j < COLS; j++)
        printf("-");
    printf("+\n");

    for(i = 0; i < ROWS; i++)
    {
        printf("|");
        for(j = 0; j < COLS; j++)
            printf("%c", grid[i][j]);
        printf("|\n");
    }

    printf("+");
    for(j = 0; j < COLS; j++)
        printf("-");
    printf("+\n");
}

void showGridWithNumbers()
{
    int i, j;
    printf("   ");
    for(j = 0; j < COLS; j++)
    {
        if(j % 10 == 0)
            printf("%d", j / 10);
        else
            printf(" ");
    }
    printf("\n");

    printf("   ");
    for(j = 0; j < COLS; j++)
        printf("%d", j % 10);
    printf("\n");

    printf("  +");
    for(j = 0; j < COLS; j++)
        printf("-");
    printf("+\n");

    for(i = 0; i < ROWS; i++)
    {
        printf("%2d|", i);
        for(j = 0; j < COLS; j++)
            printf("%c", grid[i][j]);
        printf("|\n");
    }

    printf("  +");
    for(j = 0; j < COLS; j++)
        printf("-");
    printf("+\n");
}