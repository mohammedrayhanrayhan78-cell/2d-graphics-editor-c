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

int valid_x(int x) { return x >= 0 && x < COLS; }
int valid_y(int y) { return y >= 0 && y < ROWS; }

void display_canvas() {
    printf("+");
    for (int j = 0; j < COLS; j++) printf("-");
    printf("+\n");

    for (int i = 0; i < ROWS; i++) {
        printf("|");
        for (int j = 0; j < COLS; j++)
            printf("%c", canvas[i][j]);
        printf("|\n");
    }

    printf("+");
    for (int j = 0; j < COLS; j++) printf("-");
    printf("+\n");
}

void display_canvas_with_coords() {
    // Column numbers (tens)
    printf("   ");
    for (int j = 0; j < COLS; j++)
        printf("%c", (j % 10 == 0) ? ('0' + (j / 10)) : ' ');
    printf("\n");

    // Column numbers (units)
    printf("   ");
    for (int j = 0; j < COLS; j++)
        printf("%d", j % 10);
    printf("\n");

    // Top border
    printf("  +");
    for (int j = 0; j < COLS; j++) printf("-");
    printf("+\n");

    // Rows with row numbers
    for (int i = 0; i < ROWS; i++) {
        printf("%2d|", i);
        for (int j = 0; j < COLS; j++)
            printf("%c", canvas[i][j]);
        printf("|\n");
    }

    // Bottom border
    printf("  +");
    for (int j = 0; j < COLS; j++) printf("-");
    printf("+\n");
}