#ifndef CANVAS_H
#define CANVAS_H

#define ROWS 24
#define COLS 80

extern char grid[ROWS][COLS];

void setupGrid();
void resetGrid();
void showGrid();
void showGridWithNumbers();
int checkX(int x);
int checkY(int y);

#endif