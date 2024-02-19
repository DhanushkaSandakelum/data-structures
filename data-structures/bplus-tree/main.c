#include <stdio.h>
#include <stdlib.h>

#define MAX_KEYS 3
#define MIN_KEYS ((MAX_KEYS + 1) / 2)

typedef struct Node {
    int num_keys;
    int keys[MAX_KEYS];
    struct Node* children[MAX_KEYS + 1];
    struct Node* parent;
} Node;

Node* create_node() {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->num_keys = 0;
    new_node->parent = NULL;
    for (int i = 0; i < MAX_KEYS + 1; i++)
        new_node->children[i] = NULL;
    return new_node;
}

void insert_in_leaf(Node* leaf, int key) {
    int i = leaf->num_keys - 1;
    while (i >= 0 && leaf->keys[i] > key) {
        leaf->keys[i + 1] = leaf->keys[i];
        i--;
    }
    leaf->keys[i + 1] = key;
    leaf->num_keys++;
}

void split_node(Node* node, Node** parent, int key) {
    int mid = MAX_KEYS / 2;
    Node* new_node = create_node();
    new_node->parent = node->parent;
    for (int i = mid + 1; i < MAX_KEYS + 1; i++) {
        new_node->keys[i - (mid + 1)] = node->keys[i];
        new_node->children[i - (mid + 1)] = node->children[i];
        if (new_node->children[i - (mid + 1)] != NULL)
            new_node->children[i - (mid + 1)]->parent = new_node;
    }
    new_node->num_keys = MAX_KEYS - (mid + 1);
    node->num_keys = mid;
    if (node->parent == NULL) {
        *parent = create_node();
        node->parent = *parent;
    }
    insert_in_leaf((*parent), node->keys[mid]);
    (*parent)->children[0] = node;
    (*parent)->children[1] = new_node;
}

void insert(Node** root, int key) {
    if (*root == NULL) {
        *root = create_node();
        insert_in_leaf(*root, key);
    } else {
        Node* cur_node = *root;
        while (cur_node->children[0] != NULL) {
            int i = cur_node->num_keys - 1;
            while (i >= 0 && cur_node->keys[i] > key)
                i--;
            cur_node = cur_node->children[i + 1];
        }
        insert_in_leaf(cur_node, key);
        if (cur_node->num_keys == MAX_KEYS) {
            Node* parent;
            split_node(cur_node, &parent, key);
            while (parent->parent != NULL) {
                cur_node = parent;
                parent = cur_node->parent;
                if (cur_node->num_keys == MAX_KEYS)
                    split_node(cur_node, &parent, cur_node->keys[MAX_KEYS / 2]);
                else
                    break;
            }
            if (parent->num_keys == MAX_KEYS)
                split_node(parent, root, parent->keys[MAX_KEYS / 2]);
        }
    }
}

void delete_leaf_key(Node* leaf, int key) {
    int i = 0;
    while (i < leaf->num_keys && leaf->keys[i] != key)
        i++;
    if (i == leaf->num_keys)
        return;
    for (; i < leaf->num_keys - 1; i++)
        leaf->keys[i] = leaf->keys[i + 1];
    leaf->num_keys--;
}

void merge_nodes(Node* left, Node* right) {
    for (int i = 0; i < right->num_keys; i++) {
        left->keys[left->num_keys + i] = right->keys[i];
        left->children[left->num_keys + i] = right->children[i];
        if (left->children[left->num_keys + i] != NULL)
            left->children[left->num_keys + i]->parent = left;
    }
    left->children[left->num_keys + right->num_keys] = right->children[right->num_keys];
    if (left->children[left->num_keys + right->num_keys] != NULL)
        left->children[left->num_keys + right->num_keys]->parent = left;
    left->num_keys += right->num_keys;
    free(right);
}

void delete_key(Node* node, int key);

void adjust_after_delete(Node* node) {
    if (node->num_keys < MIN_KEYS && node->parent != NULL) {
        Node* parent = node->parent;
        int index = 0;
        while (parent->children[index] != node)
            index++;
        Node* left_sibling = (index > 0) ? parent->children[index - 1] : NULL;
        Node* right_sibling = (index < parent->num_keys) ? parent->children[index + 1] : NULL;

        if (left_sibling != NULL && left_sibling->num_keys > MIN_KEYS) {
            // Borrow from left sibling
            for (int i = node->num_keys; i > 0; i--) {
                node->keys[i] = node->keys[i - 1];
                node->children[i + 1] = node->children[i];
            }
            node->children[1] = node->children[0];
            node->keys[0] = parent->keys[index - 1];
            node->num_keys++;
            parent->keys[index - 1] = left_sibling->keys[left_sibling->num_keys - 1];
            left_sibling->num_keys--;
        } else if (right_sibling != NULL && right_sibling->num_keys > MIN_KEYS) {
            // Borrow from right sibling
            node->keys[node->num_keys] = parent->keys[index];
            node->children[node->num_keys + 1] = right_sibling->children[0];
            node->num_keys++;
            parent->keys[index] = right_sibling->keys[0];
            for (int i = 0; i < right_sibling->num_keys - 1; i++) {
                right_sibling->keys[i] = right_sibling->keys[i + 1];
                right_sibling->children[i] = right_sibling->children[i + 1];
            }
            right_sibling->children[right_sibling->num_keys - 1] = right_sibling->children[right_sibling->num_keys];
            right_sibling->num_keys--;
        } else {
            // Merge with a sibling
            if (left_sibling != NULL) {
                merge_nodes(left_sibling, node);
                for (int i = index; i < parent->num_keys - 1; i++) {
                    parent->keys[i] = parent->keys[i + 1];
                    parent->children[i + 1] = parent->children[i + 2];
                }
                parent->num_keys--;
                free(node);
                node = left_sibling;
            } else {
                merge_nodes(node, right_sibling);
                for (int i = index; i < parent->num_keys - 1; i++) {
                    parent->keys[i] = parent->keys[i + 1];
                    parent->children[i + 1] = parent->children[i + 2];
                }
                parent->num_keys--;
            }
            adjust_after_delete(parent);
        }
    }
}

void delete_key(Node* node, int key) {
    int i = 0;
    while (i < node->num_keys && node->keys[i] < key)
        i++;
    if (i < node->num_keys && node->keys[i] == key) {
        if (node->children[0] != NULL) {
            Node* predecessor = node->children[i];
            while (predecessor->children[predecessor->num_keys] != NULL)
                predecessor = predecessor->children[predecessor->num_keys];
            int predecessor_key = predecessor->keys[predecessor->num_keys - 1];
            delete_leaf_key(node, predecessor_key);
            adjust_after_delete(predecessor);
        } else {
            delete_leaf_key(node, key);
            adjust_after_delete(node);
        }
    } else if (node->children[0] != NULL) {
        delete_key(node->children[i], key);
    }
}

void delete(Node** root, int key) {
    if (*root == NULL)
        return;
    delete_key(*root, key);
    if ((*root)->num_keys == 0) {
        Node* new_root = (*root)->children[0];
        if (new_root != NULL)
            new_root->parent = NULL;
        free(*root);
        *root = new_root;
    }
}

void inorder_traversal(Node* root) {
    if (root != NULL) {
        for (int i = 0; i < root->num_keys; i++) {
            inorder_traversal(root->children[i]);
            printf("%d ", root->keys[i]);
        }
        inorder_traversal(root->children[root->num_keys]);
    }
}

int main() {
    Node* root = NULL;
    insert(&root, 10);
    insert(&root, 20);
    insert(&root, 5);
    insert(&root, 30);
    insert(&root, 15);

    printf("Inorder traversal of B+ Tree: ");
    inorder_traversal(root);
    printf("\n");

    delete(&root, 15);

    printf("After deleting 15, Inorder traversal of B+ Tree: ");
    inorder_traversal(root);
    printf("\n");

    return 0;
}