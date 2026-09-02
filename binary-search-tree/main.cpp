main.cpp

#include "CustomBST.h"
#include <vector>
#include <iostream>

int main() {
    CustomBST myTree;

    // Array of numbers to insert
    std::vector<int> dataPoints = { 7, 36, 30, 3, 11, 44, 5, 32, 20, 24, 19, 54, 17, 33, 31, 34,
                               13, 64, 18, 27, 12, 29, 74, 35, 15, 4, 40, 84, 39, 28, 1, 23,
                               22, 94, 26, 8, 14, 9, 25, 104, 10, 21, 43, 37, 16, 57, 38, 66, 6, 2 };

    std::cout << "Custom Binary Search Tree Visualization" << std::endl;
    std::cout << "======================================" << std::endl;

    // Insert all numbers without printing intermediate trees
    for (int i = 0; i < dataPoints.size(); i++) {
        myTree.insertValue(dataPoints[i]);
    }

    // Only print the final tree
    std::cout << "\nFinal tree after all insertions:" << std::endl;
    myTree.displayTree();

    std::cout << "Final tree has " << myTree.getNodeCount() << " nodes." << std::endl;

    // Keep console window open
    std::cout << "Press Enter to exit...";
    std::cin.get();

    return 0;
}