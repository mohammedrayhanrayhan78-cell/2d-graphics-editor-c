#ifndef SHAPES_H
#define SHAPES_H

#define MAX_SHAPES 100

typedef enum {
    LINE,
    RECTANGLE,
    CIRCLE,
    TRIANGLE
} ShapeType;

typedef struct {
    int id;
    ShapeType type;
    int filled;            // 0 = outline only, 1 = filled
    int x1, y1, x2, y2;
    int x3, y3;            // third point for triangle
    int cx, cy, r;         // circle: center + radius
} Shape;

extern Shape shapes[MAX_SHAPES];
extern int shape_count;

/* drawing */
void draw_line(int x1, int y1, int x2, int y2);
void draw_rectangle(int x, int y, int w, int h, int filled);
void draw_circle(int cx, int cy, int r, int filled);
void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3);

void redraw_all();

/* CRUD */
int  add_shape(Shape s);
int  delete_shape(int id);
int  modify_shape(int id, Shape new_data);
void list_shapes();

/* undo */
void undo_last();

#endif