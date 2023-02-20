#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"

// 从文件中读取严格二进制树
void read_tree(char* filename, Node** root) {
    FILE* fp;
    char buffer[BUFSIZ];
    char *token, *saveptr;
    int index;
    Node* node;

    fp = fopen(filename, "r");
    if (!fp) {
        printf("Failed to open file %s\n", filename);
        exit(1);
    }

    *root = (Node*) malloc(sizeof(Node));
    (*root)->parent = NULL;
    node = *root;

    while (fgets(buffer, BUFSIZ, fp)) {
        index = 0;
        token = strtok_r(buffer, " ", &saveptr);
        while (token) {
            if (index == 0) {
                if (strcmp(token, "0") == 0) {
                    node->left = (Node*) malloc(sizeof(Node));
                    node->left->parent = node;
                    node = node->left;
                } else if (strcmp(token, "1") == 0) {
                    node->right = (Node*) malloc(sizeof(Node));
                    node->right->parent = node;
                    node = node->right;
                } else {
                    printf("Invalid input: %s\n", token);
                    exit(1);
                }
            } else if (index == 1) {
                node->rect->x = atoi(token);
            } else if (index == 2) {
                node->rect->y = atoi(token);
            } else if (index == 3) {
                node->rect->width = atoi(token);
            } else if (index == 4) {
                node->rect->height = atoi(token);
            } else {
                printf("Invalid input: %s\n", token);
                exit(1);
            }

            token = strtok_r(NULL, " ", &saveptr);
            index++;
        }

        node = node->parent;
        while (node && node->right) {
            node = node->parent;
        }
    }

    fclose(fp);
}
