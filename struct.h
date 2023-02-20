#ifndef STRUCT_H
#define STRUCT_H

typedef struct Rect {
    int id;
    int x;
    int y;
    int width;
    int height;
} Rect;

typedef struct Node {
    struct Node* parent;
    struct Node* left;
    struct Node* right;
    Rect* rect;
} Node;

typedef struct Placement {
    Rect* rect;
    int x;
    int y;
} Placement;

#endif