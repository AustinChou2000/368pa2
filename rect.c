#include "struct.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>




Rect* create_rect(int label, int w, int h) {
    Rect* rect = (Rect*)malloc(sizeof(Rect));
    rect->id = label;
    rect->width = w;
    rect->height = h;
    rect->x = 0;
    rect->y = 0;
    return rect;
}

void free_rect(Rect* rect) {
    free(rect);
}

void print_rect(Rect* rect) {
    printf("ID: %d, X: %d, Y: %d, Width: %d, Height: %d\n", rect->id, rect->x, rect->y, rect->width, rect->height);
}

bool does_rect_fit(Rect* rect, int w, int h) {
    return (rect->width <= w && rect->height <= h);
}

void rotate_rect(Rect* rect) {
    int temp = rect->width;
    rect->width = rect->height;
    rect->height = temp;
}

RectList* create_rect_list() {
    RectList* new_list = (RectList*)malloc(sizeof(RectList));
    if (new_list == NULL) {
        fprintf(stderr, "Error: memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    new_list->head = NULL;
    new_list->tail = NULL;
    new_list->size = 0;
    return new_list;
}

void free_rect_list(RectList* list) {
    RectNode* current = list->head;
    while (current != NULL) {
        RectNode* temp = current;
        current = current->next;
        free(temp->rect);
        free(temp);
    }
    free(list);
}

void add_rect(RectList* list, Rect* rect) {
    RectNode* new_node = (RectNode*)malloc(sizeof(RectNode));
    if (new_node == NULL) {
        fprintf(stderr, "Error: memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    new_node->rect = rect;
    new_node->next = NULL;

    if (list->head == NULL) {
        list->head = new_node;
    } else {
        list->tail->next = new_node;
    }

    list->tail = new_node;
    list->size++;
}

Rect* get_rect_by_label(RectList* list, int label) {
    RectNode* current = list->head;
    while (current != NULL) {
        if (current->rect->id == label) {
            return current->rect;
        }
        current = current->next;
    }
    return NULL;
}

void remove_rect(RectList* list, Rect* rect) {
    if (list == NULL || rect == NULL) {
        return;
    }
    RectNode* current = list->head;
    RectNode* prev = NULL;
    while (current != NULL) {
        if (current->rect == rect) {
            if (prev == NULL) {
                // Removing head node
                list->head = current->next;
                if (list->tail == current) {
                    list->tail = NULL;
                }
            } else if (current->next == NULL) {
                // Removing tail node
                list->tail = prev;
                prev->next = NULL;
            } else {
                // Removing node in the middle
                prev->next = current->next;
            }
            free(current);
            list->size--;
            break;
        }
        prev = current;
        current = current->next;
    }
}



