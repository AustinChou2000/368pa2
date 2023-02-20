#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <stdlib.h>
#include "struct.h"

// 函式宣告
int read_input(char *filename, rect_node **rect_list, tree_node **root);
void write_output(char *filename, tree_node *root, rect_node *rect_list);

#endif
