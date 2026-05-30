#include <stdio.h>
#include "canvas.h"
#include "fileio.h"

void saveToFile(const char *name)
{
    int i, j;
    FILE *fp = fopen(name, "w");
    if(fp == NULL)
    {
        printf("Could not open file\n");
        return;
    }

    fprintf(fp, "+");
    for(j = 0; j < COLS; j++)
        fprintf(fp, "-");
    fprintf(fp, "+\n");

    for(i = 0; i < ROWS; i++)
    {
        fprintf(fp, "|");
        for(j = 0; j < COLS; j++)
            fprintf(fp, "%c", grid[i][j]);
        fprintf(fp, "|\n");
    }

    fprintf(fp, "+");
    for(j = 0; j < COLS; j++)
        fprintf(fp, "-");
    fprintf(fp, "+\n");

    fclose(fp);
    printf("Saved to %s\n", name);
}

void loadFromFile(const char *name)
{
    int i, j;
    FILE *fp = fopen(name, "r");
    char line[COLS + 10];

    if(fp == NULL)
    {
        printf("File not found\n");
        return;
    }

    fgets(line, sizeof(line), fp);

    for(i = 0; i < ROWS; i++)
    {
        if(fgets(line, sizeof(line), fp) == NULL)
            break;
        for(j = 0; j < COLS; j++)
            grid[i][j] = line[j+1];
    }

    fclose(fp);
    printf("Loaded from %s\n", name);
}