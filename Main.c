#include <stdio.h>
#include <stdlib.h>
#include "canvas.h"
#include "shapes.h"
#include "fileio.h"

void clearInput()
{
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

int getNum(const char *msg)
{
    int n;
    printf("%s", msg);
    scanf("%d", &n);
    clearInput();
    return n;
}

void addMenu()
{
    int choice, fill;
    Shape s;

    printf("\n1. Line\n2. Rectangle\n3. Circle\n4. Triangle\n");
    choice = getNum("Enter choice: ");

    s.isFilled = 0;
    s.x1 = s.y1 = s.x2 = s.y2 = s.x3 = s.y3 = 0;
    s.cx = s.cy = s.radius = 0;
    s.num = 0;

    if(choice == 1)
    {
        s.kind = LINE;
        printf("X range: 0 to %d, Y range: 0 to %d\n", COLS-1, ROWS-1);
        s.x1 = getNum("x1: ");
        s.y1 = getNum("y1: ");
        s.x2 = getNum("x2: ");
        s.y2 = getNum("y2: ");
    }
    else if(choice == 2)
    {
        s.kind = RECTANGLE;
        printf("X range: 0 to %d, Y range: 0 to %d\n", COLS-1, ROWS-1);
        s.x1 = getNum("top left x: ");
        s.y1 = getNum("top left y: ");
        s.x2 = getNum("width: ");
        s.y2 = getNum("height: ");
        fill = getNum("filled? 1=yes 0=no: ");
        s.isFilled = fill;
    }
    else if(choice == 3)
    {
        s.kind = CIRCLE;
        printf("X range: 0 to %d, Y range: 0 to %d\n", COLS-1, ROWS-1);
        s.cx = getNum("center x: ");
        s.cy = getNum("center y: ");
        s.radius = getNum("radius: ");
        fill = getNum("filled? 1=yes 0=no: ");
        s.isFilled = fill;
    }
    else if(choice == 4)
    {
        s.kind = TRIANGLE;
        printf("X range: 0 to %d, Y range: 0 to %d\n", COLS-1, ROWS-1);
        s.x1 = getNum("point1 x: ");
        s.y1 = getNum("point1 y: ");
        s.x2 = getNum("point2 x: ");
        s.y2 = getNum("point2 y: ");
        s.x3 = getNum("point3 x: ");
        s.y3 = getNum("point3 y: ");
        fill = getNum("filled? 1=yes 0=no: ");
        s.isFilled = fill;
    }
    else
    {
        printf("Wrong choice\n");
        return;
    }

    addShape(s);
}

void deleteMenu()
{
    int id;
    printShapes();
    if(totalShapes == 0) return;
    id = getNum("Enter ID to delete: ");
    removeShape(id);
}

void modifyMenu()
{
    int id, i, fill;
    int found = 0;
    int knd;
    Shape s;

    printShapes();
    if(totalShapes == 0) return;

    id = getNum("Enter ID to modify: ");

    knd = -1;
    for(i = 0; i < totalShapes; i++)
    {
        if(shapeList[i].num == id)
        {
            knd = shapeList[i].kind;
            found = 1;
            break;
        }
    }

    if(!found)
    {
        printf("ID not found\n");
        return;
    }

    s.isFilled = 0;
    s.x1 = s.y1 = s.x2 = s.y2 = s.x3 = s.y3 = 0;
    s.cx = s.cy = s.radius = 0;
    s.num = 0;
    s.kind = knd;

    printf("X range: 0 to %d, Y range: 0 to %d\n", COLS-1, ROWS-1);

    if(knd == LINE)
    {
        s.x1 = getNum("x1: ");
        s.y1 = getNum("y1: ");
        s.x2 = getNum("x2: ");
        s.y2 = getNum("y2: ");
    }
    else if(knd == RECTANGLE)
    {
        s.x1 = getNum("top left x: ");
        s.y1 = getNum("top left y: ");
        s.x2 = getNum("width: ");
        s.y2 = getNum("height: ");
        fill = getNum("filled? 1=yes 0=no: ");
        s.isFilled = fill;
    }
    else if(knd == CIRCLE)
    {
        s.cx = getNum("center x: ");
        s.cy = getNum("center y: ");
        s.radius = getNum("radius: ");
        fill = getNum("filled? 1=yes 0=no: ");
        s.isFilled = fill;
    }
    else if(knd == TRIANGLE)
    {
        s.x1 = getNum("point1 x: ");
        s.y1 = getNum("point1 y: ");
        s.x2 = getNum("point2 x: ");
        s.y2 = getNum("point2 y: ");
        s.x3 = getNum("point3 x: ");
        s.y3 = getNum("point3 y: ");
        fill = getNum("filled? 1=yes 0=no: ");
        s.isFilled = fill;
    }

    changeShape(id, s);
}

int main()
{
    int ch;
    char fname[100];

    setupGrid();

    printf("2D Graphics Editor\n");
    printf("Grid size: %d x %d\n\n", COLS, ROWS);

    while(1)
    {
        printf("\nMenu:\n");
        printf("1. Add shape\n");
        printf("2. Delete shape\n");
        printf("3. Modify shape\n");
        printf("4. Show grid\n");
        printf("5. Show grid with numbers\n");
        printf("6. List all shapes\n");
        printf("7. Undo\n");
        printf("8. Clear grid\n");
        printf("9. Save to file\n");
        printf("10. Load from file\n");
        printf("0. Exit\n");

        ch = getNum("Choice: ");

        if(ch == 1)
        {
            addMenu();
            showGrid();
        }
        else if(ch == 2)
        {
            deleteMenu();
            showGrid();
        }
        else if(ch == 3)
        {
            modifyMenu();
            showGrid();
        }
        else if(ch == 4)
            showGrid();
        else if(ch == 5)
            showGridWithNumbers();
        else if(ch == 6)
            printShapes();
        else if(ch == 7)
        {
            undoLast();
            showGrid();
        }
        else if(ch == 8)
        {
            setupGrid();
            totalShapes = 0;
            printf("Grid cleared\n");
            showGrid();
        }
        else if(ch == 9)
        {
            printf("Enter filename: ");
            scanf("%99s", fname);
            clearInput();
            saveToFile(fname);
        }
        else if(ch == 10)
        {
            printf("Enter filename: ");
            scanf("%99s", fname);
            clearInput();
            loadFromFile(fname);
            showGrid();
        }
        else if(ch == 0)
        {
            printf("Bye\n");
            break;
        }
        else
            printf("Invalid\n");
    }

    return 0;
}