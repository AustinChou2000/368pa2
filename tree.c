#include "struct.h"
#include "rect.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>


Node* choose_subtree(Node* root, Rect* rect);

Node* create_node(Rect* rect) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        fprintf(stderr, "Error: failed to allocate memory for new node.\n");
        exit(EXIT_FAILURE);
    }
    new_node->parent = NULL;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->rect = rect;
    return new_node;
}

void delete_node(Node* node) {
    if (node == NULL) {
        return;
    }
    delete_node(node->left);
    delete_node(node->right);
    free(node->rect);
    free(node);
}

void insert_node(Node** root, Node* node) {
    if (*root == NULL) {
        *root = node;
    } else {
        // Find the best subtree to insert the node
        Node* subtree = choose_subtree(*root, node->rect);

        if (subtree->left == NULL && subtree->right == NULL) {
            // If the selected node is a leaf, split it and insert the new node
            split_node(subtree, node);
        } else {
            // Otherwise, insert the node into the selected subtree
            insert_node(&subtree->left, node);
        }
    }
}

Node* choose_subtree(Node* root, Rect* rect) {
    if (root->left == NULL && root->right == NULL) {
        return root;
    }

    Rect* cur_rect = root->rect;
    double cur_area = area(cur_rect);
    double left_area, right_area;
    Node* left = root->left;
    Node* right = root->right;

    if (left != NULL) {
        Rect* new_rect = create_rect2(left->rect->x, left->rect->y, rect->width, rect->height);
        left_area = area(new_rect) - area(left->rect);
        free(new_rect);
    } else {
        left_area = INFINITY;
    }

    if (right != NULL) {
        Rect* new_rect = create_rect2(right->rect->x, right->rect->y, rect->width, rect->height);
        right_area = area(new_rect) - area(right->rect);
        free(new_rect);
    } else {
        right_area = INFINITY;
    }

    if (cur_area + left_area < cur_area + right_area) {
        if (left == NULL) {
            return root;
        } else {
            return choose_subtree(left, rect);
        }
    } else if (cur_area + right_area < cur_area + left_area) {
        if (right == NULL) {
            return root;
        } else {
            return choose_subtree(right, rect);
        }
    } else {
        if (area(left->rect) < area(right->rect)) {
            return choose_subtree(left, rect);
        } else {
            return choose_subtree(right, rect);
        }
    }
}
