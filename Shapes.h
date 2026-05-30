#ifndef SHAPES_H
#define SHAPES_H

#define MAX 100

#define LINE      0
#define RECTANGLE 1
#define CIRCLE    2
#define TRIANGLE  3

typedef struct
{
    int num;
    int kind;
    int isFilled;
    int x1, y1;
    int x2, y2;
    int x3, y3;
    int cx, cy, radius;
} Shape;

extern Shape shapeList[MAX];
extern int totalShapes;

void putDot(int x, int y);
void makeLine(int x1, int y1, int x2, int y2);
void makeRect(int x, int y, int w, int h, int fill);
void makeCircle(int cx, int cy, int r, int fill);
void makeTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
void redrawAll();
int addShape(Shape s);
int removeShape(int id);
int changeShape(int id, Shape newData);
void printShapes();
void undoLast();

#endif