#include <stdio.h>
#include "canvas.h"

char canvas[ROWS][COLS];

void init_canvas() {
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            canvas[i][j] = ' ';
}

void clear_canvas() {
    init_canvas();
}

void display_canvas() {
    // Top border
    printf("+");
    for (int j = 0; j < COLS; j++) printf("-");
    printf("+\n");

    // Rows
    for (int i = 0; i < ROWS; i++) {
        printf("|");
        for (int j = 0; j < COLS; j++)
            printf("%c", canvas[i][j]);
        printf("|\n");
    }

    // Bottom border
    printf("+");
    for (int j = 0; j < COLS; j++) printf("-");
    printf("+\n");
}