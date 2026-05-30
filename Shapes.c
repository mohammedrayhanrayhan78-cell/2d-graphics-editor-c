#include <stdio.h>
#include <stdlib.h>
#include "shapes.h"
#include "canvas.h"

Shape shapeList[MAX];
int totalShapes = 0;
static int idCounter = 1;

Shape undoStack[50];
int undoTop = 0;

void putDot(int x, int y)
{
    if(checkX(x) && checkY(y))
        grid[y][x] = '*';
}

static int myabs(int n)
{
    if(n < 0) return -n;
    return n;
}

void makeLine(int x1, int y1, int x2, int y2)
{
    int dx = myabs(x2 - x1);
    int dy = myabs(y2 - y1);
    int sx, sy, err, e2;

    if(x1 < x2) sx = 1; else sx = -1;
    if(y1 < y2) sy = 1; else sy = -1;

    err = dx - dy;

    while(1)
    {
        putDot(x1, y1);
        if(x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if(e2 > -dy) { err -= dy; x1 += sx; }
        if(e2 < dx)  { err += dx; y1 += sy; }
    }
}

void makeRect(int x, int y, int w, int h, int fill)
{
    int i, j;
    if(fill == 1)
    {
        for(i = y; i <= y + h; i++)
            for(j = x; j <= x + w; j++)
                putDot(j, i);
    }
    else
    {
        for(i = x; i <= x + w; i++)
        {
            putDot(i, y);
            putDot(i, y + h);
        }
        for(i = y; i <= y + h; i++)
        {
            putDot(x, i);
            putDot(x + w, i);
        }
    }
}

void makeCircle(int cx, int cy, int r, int fill)
{
    int x = 0;
    int y = r;
    int d = 1 - r;
    int i;

    while(x <= y)
    {
        if(fill == 1)
        {
            for(i = cx - x; i <= cx + x; i++) { putDot(i, cy + y); putDot(i, cy - y); }
            for(i = cx - y; i <= cx + y; i++) { putDot(i, cy + x); putDot(i, cy - x); }
        }
        else
        {
            putDot(cx + x, cy + y); putDot(cx - x, cy + y);
            putDot(cx + x, cy - y); putDot(cx - x, cy - y);
            putDot(cx + y, cy + x); putDot(cx - y, cy + x);
            putDot(cx + y, cy - x); putDot(cx - y, cy - x);
        }

        if(d < 0)
            d += 2 * x + 3;
        else
        {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}

void makeTriangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
    makeLine(x1, y1, x2, y2);
    makeLine(x2, y2, x3, y3);
    makeLine(x3, y3, x1, y1);
}

void redrawAll()
{
    int i;
    resetGrid();
    for(i = 0; i < totalShapes; i++)
    {
        Shape *s = &shapeList[i];
        if(s->kind == LINE)
            makeLine(s->x1, s->y1, s->x2, s->y2);
        else if(s->kind == RECTANGLE)
            makeRect(s->x1, s->y1, s->x2, s->y2, s->isFilled);
        else if(s->kind == CIRCLE)
            makeCircle(s->cx, s->cy, s->radius, s->isFilled);
        else if(s->kind == TRIANGLE)
            makeTriangle(s->x1, s->y1, s->x2, s->y2, s->x3, s->y3);
    }
}

static int validateInput(Shape *s)
{
    if(s->kind == LINE)
    {
        if(!checkX(s->x1) || !checkY(s->y1) || !checkX(s->x2) || !checkY(s->y2))
        {
            printf("Error: coordinates out of range. X should be 0-%d, Y should be 0-%d\n", COLS-1, ROWS-1);
            return 0;
        }
    }
    else if(s->kind == RECTANGLE)
    {
        if(!checkX(s->x1) || !checkY(s->y1) || s->x2 <= 0 || s->y2 <= 0)
        {
            printf("Error: invalid rectangle values\n");
            return 0;
        }
    }
    else if(s->kind == CIRCLE)
    {
        if(!checkX(s->cx) || !checkY(s->cy) || s->radius <= 0)
        {
            printf("Error: invalid circle values\n");
            return 0;
        }
    }
    else if(s->kind == TRIANGLE)
    {
        if(!checkX(s->x1) || !checkY(s->y1) || !checkX(s->x2) || !checkY(s->y2) || !checkX(s->x3) || !checkY(s->y3))
        {
            printf("Error: triangle points out of range. X should be 0-%d, Y should be 0-%d\n", COLS-1, ROWS-1);
            return 0;
        }
    }
    return 1;
}

int addShape(Shape s)
{
    if(totalShapes >= MAX)
    {
        printf("Cannot add more shapes\n");
        return -1;
    }
    if(!validateInput(&s))
        return -1;

    s.num = idCounter++;
    shapeList[totalShapes] = s;
    totalShapes++;

    if(undoTop < 50)
        undoStack[undoTop++] = s;

    redrawAll();
    printf("Shape added. ID = %d\n", s.num);
    return s.num;
}

int removeShape(int id)
{
    int i, j, found = 0;
    for(i = 0; i < totalShapes; i++)
    {
        if(shapeList[i].num == id)
        {
            for(j = i; j < totalShapes - 1; j++)
                shapeList[j] = shapeList[j+1];
            totalShapes--;
            redrawAll();
            printf("Shape %d deleted\n", id);
            found = 1;
            break;
        }
    }
    if(!found)
        printf("Shape %d not found\n", id);
    return found;
}

int changeShape(int id, Shape newData)
{
    int i;
    for(i = 0; i < totalShapes; i++)
    {
        if(shapeList[i].num == id)
        {
            if(!validateInput(&newData))
                return 0;
            newData.num = id;
            shapeList[i] = newData;
            redrawAll();
            printf("Shape %d updated\n", id);
            return 1;
        }
    }
    printf("Shape %d not found\n", id);
    return 0;
}

void printShapes()
{
    int i;
    if(totalShapes == 0)
    {
        printf("No shapes yet\n");
        return;
    }
    printf("\nShapes on canvas:\n");
    for(i = 0; i < totalShapes; i++)
    {
        Shape *s = &shapeList[i];
        printf("ID %d: ", s->num);
        if(s->kind == LINE)
            printf("Line from (%d,%d) to (%d,%d)\n", s->x1, s->y1, s->x2, s->y2);
        else if(s->kind == RECTANGLE)
            printf("Rectangle at (%d,%d) width=%d height=%d filled=%d\n", s->x1, s->y1, s->x2, s->y2, s->isFilled);
        else if(s->kind == CIRCLE)
            printf("Circle at (%d,%d) radius=%d filled=%d\n", s->cx, s->cy, s->radius, s->isFilled);
        else if(s->kind == TRIANGLE)
            printf("Triangle (%d,%d) (%d,%d) (%d,%d)\n", s->x1, s->y1, s->x2, s->y2, s->x3, s->y3);
    }
}

void undoLast()
{
    int i, maxId, maxIdx;
    if(totalShapes == 0)
    {
        printf("Nothing to undo\n");
        return;
    }
    maxId = -1;
    maxIdx = -1;
    for(i = 0; i < totalShapes; i++)
    {
        if(shapeList[i].num > maxId)
        {
            maxId = shapeList[i].num;
            maxIdx = i;
        }
    }
    if(maxIdx != -1)
    {
        for(i = maxIdx; i < totalShapes - 1; i++)
            shapeList[i] = shapeList[i+1];
        totalShapes--;
        redrawAll();
        printf("Undo done. Removed shape %d\n", maxId);
    }
}