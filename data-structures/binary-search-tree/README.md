# Binary Search Tree(BST)

Consider following Binary Search Tree(BST) consist of 5 nodes.

```
    5
   / \
  2   8
     / \
    6  10
```

## Operations on Binary Search Tree
### Insert
In binary search tree inserting a new node depends on the value without violating BST properties.

### Searching
There are 3 types of searching procedures in BST and each of them can be recursively defined.

#### 1. Preorder Travesal

```
5	2	8	6	10
```

#### 2. Inorder Travesal

```
2	5	6	8	10
```

#### 3. Postorder Travesal

```
2	6	10	8	5	
```

### Delete
Recursively search for the node to be deleted. If node is found, then under 2 different cases the deletion is performed. (Node found with a one child and node found with two child)

### Utilities
#### 1. Find the largest element
Traverse right most node of the tree as much as possible until there is no further right child.

#### 2. Find the smallest element
Traverse left most node of the tree as much as possible until there is no further left child.