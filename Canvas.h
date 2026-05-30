#ifndef CANVAS_H
#define CANVAS_H

#define ROWS 24
#define COLS 80

extern char canvas[ROWS][COLS];

void init_canvas();
void clear_canvas();
void display_canvas();
void display_canvas_with_coords();

int valid_x(int x);
int valid_y(int y);

#endif