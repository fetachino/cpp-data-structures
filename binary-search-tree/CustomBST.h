#ifndef CUSTOM_BST_H
#define CUSTOM_BST_H

#include <iostream>
#include <queue>
#include <string>
#include <vector>

class CustomBST {
private:
    struct TreeVertex {
        int value;
        TreeVertex* leftChild;
        TreeVertex* rightChild;

        TreeVertex(int val);
    };

    TreeVertex* rootVertex;
    int nodeCount;

    // Helper function to insert a value into the tree
    TreeVertex* insertRecursive(TreeVertex* currentVertex, int val);

    // Calculate tree depth
    int calculateTreeDepth(TreeVertex* vertex);

    void cleanupTree(TreeVertex* vertex);

public:
    CustomBST();
    ~CustomBST();

    // Insert a value into the tree
    void insertValue(int val);

    int getNodeCount() const;

    // Print the tree with underscores for connections
    void displayTree();
};

#endif // CUSTOM_BST_H#pragma once

