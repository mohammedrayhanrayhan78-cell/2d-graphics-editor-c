#include <stdio.h>
#include "canvas.h"
#include "fileio.h"

void save_canvas(const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Error: Could not save to '%s'.\n", filename);
        return;
    }

    fprintf(fp, "+");
    for (int j = 0; j < COLS; j++) fprintf(fp, "-");
    fprintf(fp, "+\n");

    for (int i = 0; i < ROWS; i++) {
        fprintf(fp, "|");
        for (int j = 0; j < COLS; j++)
            fprintf(fp, "%c", canvas[i][j]);
        fprintf(fp, "|\n");
    }

    fprintf(fp, "+");
    for (int j = 0; j < COLS; j++) fprintf(fp, "-");
    fprintf(fp, "+\n");

    fclose(fp);
    printf("Canvas saved to '%s'.\n", filename);
}

void load_canvas(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: Could not open '%s'.\n", filename);
        return;
    }

    char line[COLS + 10];

    // Skip top border line
    if (fgets(line, sizeof(line), fp) == NULL) { fclose(fp); return; }

    for (int i = 0; i < ROWS; i++) {
        if (fgets(line, sizeof(line), fp) == NULL) break;
        // line format: |<COLS chars>|
        for (int j = 0; j < COLS && line[j + 1] != '\0'; j++)
            canvas[i][j] = line[j + 1];
    }

    fclose(fp);
    printf("Canvas loaded from '%s'.\n", filename);
}