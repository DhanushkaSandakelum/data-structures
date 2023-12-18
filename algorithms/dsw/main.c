#include <stdio.h>
#include <malloc.h>

#include "./include/bst.c"
#include "./include/util_math.c"

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
    printf("\ntree height: %d\n", tree_height(tree));

    tree = create_backbone(tree);
    printf("\n--- Backbone/ Vine ---\n");
    pre_order_traversal(tree);
    printf("\ntree height: %d\n", tree_height(tree));

    printf("\nFolowing data is required for DSW calculations,\n");
    int n = total_nodes(tree);
    int res = get_perfect_tree_node_count(n);
    printf("total nodes: %d | perfect tree node count: %d | extra node count: %d\n", n, res, n - res);

    balance_backbone(tree, n);
    printf("\n--- After DSW - Balanced BST---\n");
    pre_order_traversal(tree);
    printf("\ntree height: %d\n", tree_height(tree));

    return 0;
}