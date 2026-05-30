#include <stdio.h>
#include <stdlib.h>
#include "shapes.h"
#include "canvas.h"

Shape shapes[MAX_SHAPES];
int shape_count = 0;
static int next_id = 1;

/* ── undo stack ── */
#define MAX_UNDO 50
static Shape undo_stack[MAX_UNDO];
static int undo_count = 0;

static void undo_push(Shape s) {
    if (undo_count < MAX_UNDO)
        undo_stack[undo_count++] = s;
}

/* ── helpers ── */

static void plot(int x, int y) {
    if (valid_x(x) && valid_y(y))
        canvas[y][x] = '*';
}

static int abs_val(int n) { return n < 0 ? -n : n; }

/* ── drawing primitives ── */

void draw_line(int x1, int y1, int x2, int y2) {
    int dx = abs_val(x2 - x1), dy = abs_val(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        plot(x1, y1);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 <  dx) { err += dx; y1 += sy; }
    }
}

void draw_rectangle(int x, int y, int w, int h, int filled) {
    if (filled) {
        for (int row = y; row <= y + h; row++)
            for (int col = x; col <= x + w; col++)
                plot(col, row);
    } else {
        for (int i = x; i <= x + w; i++) {
            plot(i, y);
            plot(i, y + h);
        }
        for (int i = y; i <= y + h; i++) {
            plot(x, i);
            plot(x + w, i);
        }
    }
}

void draw_circle(int cx, int cy, int r, int filled) {
    int x = 0, y = r;
    int d = 1 - r;

    while (x <= y) {
        if (filled) {
            // fill horizontal spans
            for (int i = cx - x; i <= cx + x; i++) { plot(i, cy + y); plot(i, cy - y); }
            for (int i = cx - y; i <= cx + y; i++) { plot(i, cy + x); plot(i, cy - x); }
        } else {
            plot(cx + x, cy + y); plot(cx - x, cy + y);
            plot(cx + x, cy - y); plot(cx - x, cy - y);
            plot(cx + y, cy + x); plot(cx - y, cy + x);
            plot(cx + y, cy - x); plot(cx - y, cy - x);
        }

        if (d < 0) {
            d += 2 * x + 3;
        } else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}

void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3) {
    draw_line(x1, y1, x2, y2);
    draw_line(x2, y2, x3, y3);
    draw_line(x3, y3, x1, y1);
}

/* ── redraw all ── */

void redraw_all() {
    clear_canvas();
    for (int i = 0; i < shape_count; i++) {
        Shape *s = &shapes[i];
        switch (s->type) {
            case LINE:
                draw_line(s->x1, s->y1, s->x2, s->y2);
                break;
            case RECTANGLE:
                draw_rectangle(s->x1, s->y1, s->x2, s->y2, s->filled);
                break;
            case CIRCLE:
                draw_circle(s->cx, s->cy, s->r, s->filled);
                break;
            case TRIANGLE:
                draw_triangle(s->x1, s->y1, s->x2, s->y2, s->x3, s->y3);
                break;
        }
    }
}

/* ── input validation ── */

static int validate_shape(Shape *s) {
    switch (s->type) {
        case LINE:
            if (!valid_x(s->x1) || !valid_y(s->y1) ||
                !valid_x(s->x2) || !valid_y(s->y2)) {
                printf("Error: coordinates out of bounds (X: 0-%d, Y: 0-%d).\n", COLS-1, ROWS-1);
                return 0;
            }
            break;
        case RECTANGLE:
            if (!valid_x(s->x1) || !valid_y(s->y1) ||
                !valid_x(s->x1 + s->x2) || !valid_y(s->y1 + s->y2) ||
                s->x2 <= 0 || s->y2 <= 0) {
                printf("Error: rectangle out of bounds or invalid size.\n");
                return 0;
            }
            break;
        case CIRCLE:
            if (!valid_x(s->cx) || !valid_y(s->cy) || s->r <= 0) {
                printf("Error: circle center out of bounds or invalid radius.\n");
                return 0;
            }
            break;
        case TRIANGLE:
            if (!valid_x(s->x1) || !valid_y(s->y1) ||
                !valid_x(s->x2) || !valid_y(s->y2) ||
                !valid_x(s->x3) || !valid_y(s->y3)) {
                printf("Error: triangle points out of bounds (X: 0-%d, Y: 0-%d).\n", COLS-1, ROWS-1);
                return 0;
            }
            break;
    }
    return 1;
}

/* ── CRUD ── */

int add_shape(Shape s) {
    if (shape_count >= MAX_SHAPES) {
        printf("Canvas full! Max %d shapes.\n", MAX_SHAPES);
        return -1;
    }
    if (!validate_shape(&s)) return -1;

    s.id = next_id++;
    shapes[shape_count++] = s;
    undo_push(s);   // record for undo
    redraw_all();
    printf("Shape added with ID %d.\n", s.id);
    return s.id;
}

int delete_shape(int id) {
    for (int i = 0; i < shape_count; i++) {
        if (shapes[i].id == id) {
            for (int j = i; j < shape_count - 1; j++)
                shapes[j] = shapes[j + 1];
            shape_count--;
            redraw_all();
            printf("Shape ID %d deleted.\n", id);
            return 1;
        }
    }
    printf("Shape ID %d not found.\n", id);
    return 0;
}

int modify_shape(int id, Shape new_data) {
    for (int i = 0; i < shape_count; i++) {
        if (shapes[i].id == id) {
            if (!validate_shape(&new_data)) return 0;
            new_data.id = id;
            shapes[i] = new_data;
            redraw_all();
            printf("Shape ID %d modified.\n", id);
            return 1;
        }
    }
    printf("Shape ID %d not found.\n", id);
    return 0;
}

void list_shapes() {
    if (shape_count == 0) {
        printf("No shapes on canvas.\n");
        return;
    }
    printf("\n--- Shapes on canvas ---\n");
    for (int i = 0; i < shape_count; i++) {
        Shape *s = &shapes[i];
        printf("ID %d: ", s->id);
        switch (s->type) {
            case LINE:
                printf("Line (%d,%d) -> (%d,%d)\n", s->x1, s->y1, s->x2, s->y2);
                break;
            case RECTANGLE:
                printf("Rectangle at (%d,%d) w=%d h=%d [%s]\n",
                       s->x1, s->y1, s->x2, s->y2, s->filled ? "filled" : "outline");
                break;
            case CIRCLE:
                printf("Circle center (%d,%d) r=%d [%s]\n",
                       s->cx, s->cy, s->r, s->filled ? "filled" : "outline");
                break;
            case TRIANGLE:
                printf("Triangle (%d,%d) (%d,%d) (%d,%d)\n",
                       s->x1, s->y1, s->x2, s->y2, s->x3, s->y3);
                break;
        }
    }
    printf("------------------------\n");
}

/* ── undo ── */

void undo_last() {
    if (shape_count == 0) {
        printf("Nothing to undo.\n");
        return;
    }
    // Remove last added shape (highest id)
    int max_id = -1, max_idx = -1;
    for (int i = 0; i < shape_count; i++) {
        if (shapes[i].id > max_id) {
            max_id = shapes[i].id;
            max_idx = i;
        }
    }
    if (max_idx != -1) {
        for (int j = max_idx; j < shape_count - 1; j++)
            shapes[j] = shapes[j + 1];
        shape_count--;
        redraw_all();
        printf("Undo: removed shape ID %d.\n", max_id);
    }
}