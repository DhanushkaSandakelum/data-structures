#include <stdio.h>
#include <malloc.h>
// #include <math.h>

// Math Utility
long long power(int base, int exponent)
{
    if (exponent < 0)
    {
        // Handle negative exponent if needed
        return 0; // You may want to return an error or handle it differently
    }

    long long result = 1;

    for (int i = 0; i < exponent; ++i)
    {
        result *= base;
    }

    return result;
}

double calculate_log2(double x)
{
    if (x <= 0)
    {
        // Logarithm is undefined for non-positive numbers
        printf("Error: Logarithm is undefined for non-positive numbers.\n");
        return 0.0; // You may want to return an error or handle it differently
    }

    double result = 0.0;
    while (x >= 2.0)
    {
        x /= 2.0;
        result += 1.0;
    }

    return result;
}

// Node structure
struct node
{
    int data;
    struct node *left;
    struct node *right;
};

struct node *create_node(int val)
{
    struct node *new_node = (struct node *)malloc(sizeof(struct node));
    new_node->data = val;
    new_node->left = NULL;
    new_node->right = NULL;

    return new_node;
}

// Insert
struct node *insert_node(struct node *tree, int val)
{
    // Base Case: Create a new node
    if (tree == NULL)
        return create_node(val);

    // Recursive Case: Insert the value into the appropriate subtree
    if (val < tree->data)
    {
        tree->left = insert_node(tree->left, val);
    }
    else
    {
        tree->right = insert_node(tree->right, val);
    }

    return tree;
}

// Searching
void pre_order_traversal(struct node *tree)
{
    if (tree != NULL)
    {
        printf("%d\t", tree->data);
        pre_order_traversal(tree->left);
        pre_order_traversal(tree->right);
    }
}

// Utility
int total_nodes(struct node *tree)
{
    if (tree == NULL)
        return 0;
    else
        return (total_nodes(tree->left) + total_nodes(tree->right) + 1);
}

// Rotations
void rotate_right(struct node *root)
{
    // Swap values
    int x = root->data;
    root->data = root->left->data;
    root->left->data = x;

    // Perform rotation
    struct node *temp = root->left;
    root->left = temp->left;
    temp->left = temp->right;
    temp->right = root->right;
    root->right = temp;
}

void rotate_left(struct node *root)
{
    // Swap values
    int x = root->data;
    root->data = root->right->data;
    root->right->data = x;

    // Perform rotation
    struct node *temp = root->right;
    root->right = temp->right;
    temp->right = temp->left;
    temp->left = root->left;
    root->left = temp;
}

// DSW Algorithms
struct node *create_backbone(struct node *root)
{
    struct node *temp = root;

    while (temp != NULL)
    {
        while (temp->left != NULL)
        {
            rotate_right(temp);
        }

        temp = temp->right;
    }

    return root;
}

int get_perfect_tree_node_count(int n)
{
    int perfect_count = 0;
    int i = 0;

    while (perfect_count < n)
    {
        i++;
        perfect_count = power(2, i) - 1;
    }

    int res = power(2, i - 1);

    return res - 1;
}

void balance_backbone(struct node *root, int n)
{
    int i = 0;
    int extra_node_count = n - get_perfect_tree_node_count(n);

    struct node *current_node = root;

    while (i < extra_node_count)
    {
        rotate_left(current_node);
        current_node = current_node->right;
        i++;
    }

    int iterations = get_perfect_tree_node_count(n);

    while (iterations > 0)
    {
        iterations = iterations / 2;
        i = 0;
        current_node = root;

        while (i < iterations)
        {
            rotate_left(current_node);
            current_node = current_node->right;
            i++;
        }
    }
}

int main(int argc, char const *argv[])
{
    printf("\n--- Insert --- \n");
    struct node *tree = NULL;
    tree = insert_node(tree, 5);
    insert_node(tree, 2);
    insert_node(tree, 8);
    insert_node(tree, 6);
    insert_node(tree, 10);

    printf("\n--- Before DSW - Unbalanced BST ---\n");
    pre_order_traversal(tree);
    printf("\n");

    tree = create_backbone(tree);
    printf("\n--- Backbone/ Vine ---\n");
    pre_order_traversal(tree);
    printf("\n");

    printf("\n Folowing data is required for DSW calculations,\n");
    int n = total_nodes(tree);
    int res = get_perfect_tree_node_count(n);
    printf("total nodes: %d | perfect tree node count: %d | extra node count: %d\n", n, res, n - res);

    balance_backbone(tree, n);
    printf("\n--- After DSW - Balanced BST---\n");
    pre_order_traversal(tree);
    printf("\n");

    return 0;
}