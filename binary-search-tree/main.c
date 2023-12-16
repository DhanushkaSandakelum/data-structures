#include <stdio.h>
#include <malloc.h>

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

// Searching (Pre-order, In-order, Post-order)
void pre_order_traversal(struct node *tree)
{
    if (tree != NULL)
    {
        printf("%d\t", tree->data);
        pre_order_traversal(tree->left);
        pre_order_traversal(tree->right);
    }
}

void in_order_traversal(struct node *tree)
{
    if (tree != NULL)
    {
        in_order_traversal(tree->left);
        printf("%d\t", tree->data);
        in_order_traversal(tree->right);
    }
}

void post_order_traversal(struct node *tree)
{
    if (tree != NULL)
    {
        post_order_traversal(tree->left);
        post_order_traversal(tree->right);
        printf("%d\t", tree->data);
    }
}

// Find the largest element
struct node *find_largest_element(struct node *tree)
{
    if (tree == NULL || tree->right == NULL)
        return tree;
    else
        return find_largest_element(tree->right);
}

// Find the smallest element
struct node *find_smallest_element(struct node *tree)
{
    if (tree == NULL || tree->left == NULL)
        return tree;
    else
        return find_largest_element(tree->left);
}

// Delete
struct node *delete_node(struct node *tree, int val)
{
    if (tree == NULL)
    {
        printf("Value not found in the tree\n");
        return tree;
    }

    // If value to be deleted is smaller than current tree value, go to left branch
    if (val < tree->data)
    {
        tree->left = delete_node(tree->left, val);
    }
    // If value to be deleted is larger than current tree value, go to right branch
    else if (val > tree->data)
    {
        tree->right = delete_node(tree->right, val);
    }
    // If the value to be deleted is equal to the tree value, then this node is to be deleted
    else
    {
        // If node has only one child, delete it and free space
        if (tree->left == NULL)
        {
            struct node *temp = tree->right;
            free(tree);

            return temp;
        }
        else if (tree->right == NULL)
        {
            struct node *temp = tree->left;
            free(tree);

            return temp;
        }

        // If node has two children, get the inorder successor(smallest in the right subtree)
        struct node *temp = find_smallest_element(tree->right);

        // Copy inorder successor's content to the current node
        tree->data = temp->data;

        // Delete the inorder successor
        tree->right = delete_node(tree->right, temp->data);
    }

    return tree;
}

int main(int argc, char const *argv[])
{
    printf("\n--- Insert --- \n");
    struct node *tree = insert_node(tree, 5);
    insert_node(tree, 2);
    insert_node(tree, 8);
    insert_node(tree, 6);
    insert_node(tree, 10);

    printf("\n--- Searching --- \n");
    pre_order_traversal(tree);
    printf("\n");
    in_order_traversal(tree);
    printf("\n");
    post_order_traversal(tree);
    printf("\n");

    printf("\n--- Utilities --- \n");
    struct node *ptr;
    ptr = find_largest_element(tree);
    printf("Largest element of the tree: %d\n", ptr->data);

    ptr = find_smallest_element(tree);
    printf("Smallest element of the tree: %d\n", ptr->data);

    printf("\n--- Delete --- \n");
    delete_node(tree, 8);

    pre_order_traversal(tree);
    printf("\n");

    return 0;
}
