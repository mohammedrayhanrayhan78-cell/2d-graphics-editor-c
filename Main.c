#include <stdio.h>
#include <stdlib.h>
#include "canvas.h"
#include "shapes.h"
#include "fileio.h"

/* ── input helpers ── */

static void flush_input() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static int get_int(const char *prompt) {
    int val;
    printf("%s", prompt);
    scanf("%d", &val);
    flush_input();
    return val;
}

static int get_filled() {
    printf("  Fill shape? (1=filled, 0=outline): ");
    int f; scanf("%d", &f); flush_input();
    return f;
}

/* ── sub-menus ── */

static void menu_add() {
    printf("\nShape to add:\n");
    printf("  1. Line\n  2. Rectangle\n  3. Circle\n  4. Triangle\n");
    int choice = get_int("Choice: ");

    Shape s = {0};

    switch (choice) {
        case 1:
            s.type = LINE;
            printf("  Canvas: X(0-%d) Y(0-%d)\n", COLS-1, ROWS-1);
            s.x1 = get_int("  Start X: ");
            s.y1 = get_int("  Start Y: ");
            s.x2 = get_int("  End X:   ");
            s.y2 = get_int("  End Y:   ");
            break;
        case 2:
            s.type = RECTANGLE;
            printf("  Canvas: X(0-%d) Y(0-%d)\n", COLS-1, ROWS-1);
            s.x1 = get_int("  Top-left X: ");
            s.y1 = get_int("  Top-left Y: ");
            s.x2 = get_int("  Width:      ");
            s.y2 = get_int("  Height:     ");
            s.filled = get_filled();
            break;
        case 3:
            s.type = CIRCLE;
            printf("  Canvas: X(0-%d) Y(0-%d)\n", COLS-1, ROWS-1);
            s.cx = get_int("  Center X: ");
            s.cy = get_int("  Center Y: ");
            s.r  = get_int("  Radius:   ");
            s.filled = get_filled();
            break;
        case 4:
            s.type = TRIANGLE;
            printf("  Canvas: X(0-%d) Y(0-%d)\n", COLS-1, ROWS-1);
            printf("  Point 1:\n");
            s.x1 = get_int("    X: "); s.y1 = get_int("    Y: ");
            printf("  Point 2:\n");
            s.x2 = get_int("    X: "); s.y2 = get_int("    Y: ");
            printf("  Point 3:\n");
            s.x3 = get_int("    X: "); s.y3 = get_int("    Y: ");
            break;
        default:
            printf("Invalid choice.\n");
            return;
    }

    add_shape(s);
}

static void menu_delete() {
    list_shapes();
    if (shape_count == 0) return;
    int id = get_int("Enter shape ID to delete: ");
    delete_shape(id);
}

static void menu_modify() {
    list_shapes();
    if (shape_count == 0) return;
    int id = get_int("Enter shape ID to modify: ");

    ShapeType t = -1;
    for (int i = 0; i < shape_count; i++)
        if (shapes[i].id == id) { t = shapes[i].type; break; }

    if (t == -1) { printf("ID not found.\n"); return; }

    Shape s = {0};
    s.type = t;

    printf("Enter new values (Canvas: X 0-%d, Y 0-%d):\n", COLS-1, ROWS-1);
    switch (t) {
        case LINE:
            s.x1 = get_int("  Start X: "); s.y1 = get_int("  Start Y: ");
            s.x2 = get_int("  End X:   "); s.y2 = get_int("  End Y:   ");
            break;
        case RECTANGLE:
            s.x1 = get_int("  Top-left X: "); s.y1 = get_int("  Top-left Y: ");
            s.x2 = get_int("  Width:      "); s.y2 = get_int("  Height:     ");
            s.filled = get_filled();
            break;
        case CIRCLE:
            s.cx = get_int("  Center X: ");
            s.cy = get_int("  Center Y: ");
            s.r  = get_int("  Radius:   ");
            s.filled = get_filled();
            break;
        case TRIANGLE:
            printf("  Point 1:\n");
            s.x1 = get_int("    X: "); s.y1 = get_int("    Y: ");
            printf("  Point 2:\n");
            s.x2 = get_int("    X: "); s.y2 = get_int("    Y: ");
            printf("  Point 3:\n");
            s.x3 = get_int("    X: "); s.y3 = get_int("    Y: ");
            break;
    }

    modify_shape(id, s);
}

/* ── main ── */

int main() {
    init_canvas();

    printf("╔══════════════════════════════╗\n");
    printf("║   2D Graphics Editor in C    ║\n");
    printf("╚══════════════════════════════╝\n");
    printf("Canvas size: %d cols x %d rows\n", COLS, ROWS);

    int running = 1;
    while (running) {
        printf("\n--- MENU ---\n");
        printf("  1. Add shape\n");
        printf("  2. Delete shape\n");
        printf("  3. Modify shape\n");
        printf("  4. Display canvas\n");
        printf("  5. Display canvas with coordinates\n");
        printf("  6. List shapes\n");
        printf("  7. Undo last action\n");
        printf("  8. Clear canvas\n");
        printf("  9. Save canvas to file\n");
        printf(" 10. Load canvas from file\n");
        printf("  0. Exit\n");

        int choice = get_int("Choice: ");

        switch (choice) {
            case 1: menu_add();    display_canvas(); break;
            case 2: menu_delete(); display_canvas(); break;
            case 3: menu_modify(); display_canvas(); break;
            case 4: display_canvas(); break;
            case 5: display_canvas_with_coords(); break;
            case 6: list_shapes(); break;
            case 7: undo_last();   display_canvas(); break;
            case 8:
                init_canvas();
                shape_count = 0;
                printf("Canvas cleared.\n");
                display_canvas();
                break;
            case 9: {
                char fname[100];
                printf("Enter filename (e.g. output.txt): ");
                scanf("%99s", fname); flush_input();
                save_canvas(fname);
                break;
            }
            case 10: {
                char fname[100];
                printf("Enter filename to load: ");
                scanf("%99s", fname); flush_input();
                load_canvas(fname);
                display_canvas();
                break;
            }
            case 0:
                printf("Goodbye!\n");
                running = 0;
                break;
            default:
                printf("Invalid option.\n");
        }
    }

    return 0;
}