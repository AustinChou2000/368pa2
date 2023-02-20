#include "tree.h"

Node* create_node(Rect* rect) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;
    node->rect = rect;

    return node;
}

void destroy_node(Node* node) {
    if (node != NULL) {
        free(node);
    }
}

Node* insert_node(Node* root, Node* node) {
    if (root == NULL) {
        return node;
    }

    Node* new_root = NULL;

    if (root->left == NULL && root->right == NULL) {
        root->left = node;
        node->parent = root;
        new_root = split_node(root, node);
    } else {
        int left_area = calculate_total_area(root->left);
        int right_area = calculate_total_area(root->right);

        if (left_area == right_area) {
            if (root->left != NULL) {
                new_root = insert_node(root->left, node);
            } else {
                new_root = insert_node(root->right, node);
            }
        } else if (left_area < right_area) {
            new_root = insert_node(root->left, node);
        } else {
            new_root = insert_node(root->right, node);
        }
    }

    if (new_root != NULL) {
        return new_root;
    }

    return root;
}

int find_split_dimension(Node* node) {
    int dim = node->rect.dims;
    double max_spread = -1.0;
    int split_dim = 0;
    for (int i = 0; i < dim; i++) {
        double min_value = node->rect.l[i];
        double max_value = node->rect.h[i];
        for (int j = 0; j < node->count; j++) {
            if (node->child[j]->rect.l[i] < min_value) {
                min_value = node->child[j]->rect.l[i];
            }
            if (node->child[j]->rect.h[i] > max_value) {
                max_value = node->child[j]->rect.h[i];
            }
        }
        double spread = max_value - min_value;
        if (spread > max_spread) {
            max_spread = spread;
            split_dim = i;
        }
    }
    return split_dim;
}

double find_median(Node* node, int split_dimension) {
    int count = node->count;
    double* values = (double*)malloc(count * sizeof(double));
    for (int i = 0; i < count; i++) {
        values[i] = node->child[i]->rect.l[split_dimension];
    }
    qsort(values, count, sizeof(double), compare_double);
    double median;
    if (count % 2 == 0) {
        median = (values[count / 2 - 1] + values[count / 2]) / 2.0;
    } else {
        median = values[count / 2];
    }
    free(values);
    return median;
}

Node* split_node_on_dimension(Node* node, int split_dimension, double split_value) {
    Rect **rects = node->rects;
    int num_rects = node->num_rects;

    // Create two new nodes
    Node* new_node1 = create_node(rects[0]);
    Node* new_node2 = create_node(rects[0]);

    // Partition the rects into the new nodes
    for (int i = 1; i < num_rects; i++) {
        Rect *r = rects[i];
        if (r->coords[split_dimension] < split_value) {
            insert_node(new_node1, r);
        } else {
            insert_node(new_node2, r);
        }
    }

    // Destroy the old node
    destroy_node(node);

    // Return the two new nodes
    Node** children = new Node*[2];
    children[0] = new_node1;
    children[1] = new_node2;
    return create_node(children, 2);
}
