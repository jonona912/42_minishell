#include <stdio.h>
#include <stdlib.h>

// Define the BST node structure
typedef struct Node {
    int data;
    struct Node* left;
    struct Node* right;
} Node;

// Function to create a new node
Node* createNode(int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = value;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Function to insert a node into the BST
Node* insert(Node* root, int value) {
    if (root == NULL) {
        return createNode(value); // Base case: create node if tree is empty
    }
    if (value < root->data) {
        root->left = insert(root->left, value); // Recurse left
    } else if (value > root->data) {
        root->right = insert(root->right, value); // Recurse right
    }
    return root;
}

// Function to search for a value in the BST
Node* search(Node* root, int key) {
    if (root == NULL || root->data == key) {
        return root; // Found or tree is empty
    }
    if (key < root->data) {
        return search(root->left, key); // Search left subtree
    } else {
        return search(root->right, key); // Search right subtree
    }
}

// Function for in-order traversal (prints nodes in sorted order)
void inorderTraversal(Node* root) {
    if (root != NULL) {
        inorderTraversal(root->left);
        printf("%d ", root->data);
        inorderTraversal(root->right);
    }
}

void freetree(Node *root)
{
	if (root)
	{
		freetree(root->left);
		freetree(root->right);
		free(root);
	}
}

// Function to free the entire BST
void freeTree(Node* root) {
	if (root != NULL) {
		freeTree(root->left);
		freeTree(root->right);
		free(root);
	}
}
// Main function to test the BST
int main() {
    Node* root = NULL;
    int values[] = {8, 3, 10, 1, 1, 6, 14, 4, 7};
    int size = sizeof(values) / sizeof(values[0]);

    // Insert values into the BST
    for (int i = 0; i < size; i++) {
        root = insert(root, values[i]);
    }

    // Print the BST (in-order traversal)
    printf("In-order traversal (sorted order): ");
    inorderTraversal(root); // Output: 1 3 4 6 7 8 10 14

    // Search for a value (e.g., 6)
    int searchKey = 6;
    Node* result = search(root, searchKey);
    if (result != NULL) {
        printf("\n%d found in the BST!\n", searchKey);
    } else {
        printf("\n%d not found in the BST.\n", searchKey);
    }
	freetree(root);
    // Free memory (optional, for completeness)
    // Note: In a real program, you'd need a proper tree deletion function.
    return 0;
}