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

void add_node_to_parent(Node* root, Node* node, Node* new_node) {
    // Check if the root node is null, which means the tree is empty
    if (root == NULL) {
        root = new_node;
        return;
    }

    // Find the parent node of the node
    Node* parent = find_parent(root, node);

    // If the parent is not full, insert the new node into it
    if (parent->num_rects < MAX_RECTS) {
        insert_node(parent, new_node->rects[0]);
        insert_node(parent, new_node->rects[1]);
        destroy_node(new_node);
    } else {
        // Otherwise, split the parent node
        int split_dimension;
        double split_value;
        find_split_dimension(parent, &split_dimension);
        find_median(parent, split_dimension, &split_value);
        Node* new_parent = split_node_on_dimension(parent, split_dimension, split_value);

        // Add the two new nodes to the new parent
        add_node_to_parent(root, parent, new_node);
        add_node_to_parent(root, new_parent, new_node);
    }
}

Node* find_parent(Node* root, Node* node) {
    // If the root is null or has no children, return null
    if (root == NULL || root->children == NULL) {
        return NULL;
    }

    // Check if the node is a child of the root
    for (int i = 0; i < root->num_children; i++) {
        if (root->children[i] == node) {
            return root;
        }
    }

    // Recursively search for the node's parent
    for (int i = 0; i < root->num_children; i++) {
        Node* parent = find_parent(root->children[i], node);
        if (parent != NULL) {
            return parent;
        }
    }

    return NULL;
}

void search_tree(Node* node, Rect* rect, int* count, Rect** results, int max_results) {
    if (node->leaf) {
        for (int i = 0; i < node->num_rects; i++) {
            if (intersects(node->rects[i], rect)) {
                if (*count == max_results) {
                    // results array is full, cannot add more
                    return;
                }
                results[*count] = node->rects[i];
                (*count)++;
            }
        }
    } else {
        for (int i = 0; i < node->num_children; i++) {
            if (intersects(node->children[i]->boundary, rect)) {
                search_tree(node->children[i], rect, count, results, max_results);
            }
        }
    }
}

