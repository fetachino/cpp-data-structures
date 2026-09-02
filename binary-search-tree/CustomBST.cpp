 #include "CustomBST.h"
#include <iomanip>

// TreeVertex constructor implementation
CustomBST::TreeVertex::TreeVertex(int val) : value(val), leftChild(nullptr), rightChild(nullptr) {}

// Constructor
CustomBST::CustomBST() : rootVertex(nullptr), nodeCount(0) {}

// Destructor
CustomBST::~CustomBST() {
    cleanupTree(rootVertex);
}

// Helper function to insert a value into the tree
CustomBST::TreeVertex* CustomBST::insertRecursive(TreeVertex* currentVertex, int val) {
    if (currentVertex == nullptr) {
        nodeCount++;
        return new TreeVertex(val);
    }

    if (val < currentVertex->value) {
        currentVertex->leftChild = insertRecursive(currentVertex->leftChild, val);
    }
    else if (val > currentVertex->value) {
        currentVertex->rightChild = insertRecursive(currentVertex->rightChild, val);
    }
    // Ignore duplicates

    return currentVertex;
}

// Calculate tree depth
int CustomBST::calculateTreeDepth(TreeVertex* vertex) {
    if (vertex == nullptr) {
        return 0;
    }

    int leftDepth = calculateTreeDepth(vertex->leftChild);
    int rightDepth = calculateTreeDepth(vertex->rightChild);

    return std::max(leftDepth, rightDepth) + 1;
}

void CustomBST::cleanupTree(TreeVertex* vertex) {
    if (vertex != nullptr) {
        cleanupTree(vertex->leftChild);
        cleanupTree(vertex->rightChild);
        delete vertex;
    }
}

// Insert a value into the tree
void CustomBST::insertValue(int val) {
    rootVertex = insertRecursive(rootVertex, val);
}

int CustomBST::getNodeCount() const {
    return nodeCount;
}

// Print the tree with underscores for connections
void CustomBST::displayTree() {
    if (rootVertex == nullptr) {
        std::cout << "Tree is empty" << std::endl;
        return;
    }

    int depth = calculateTreeDepth(rootVertex);

    // Calculate positions for nodes at each level
    std::vector<std::vector<std::string>> levelLines(depth);
    std::vector<std::vector<int>> nodePositions(depth);

    // Calculate the maximum width needed
    int maxWidth = (1 << depth) * 4;

    // Fill with blank spaces
    for (int i = 0; i < depth; i++) {
        levelLines[i] = std::vector<std::string>(1, std::string(maxWidth, ' '));
        nodePositions[i] = std::vector<int>();
    }

    // Queue for level-order traversal with position info
    struct QueueItem {
        TreeVertex* vertex;
        int level;
        int position;

        QueueItem(TreeVertex* v, int l, int p) : vertex(v), level(l), position(p) {}
    };

    std::queue<QueueItem> vertexQueue;
    vertexQueue.push(QueueItem(rootVertex, 0, maxWidth / 2));

    while (!vertexQueue.empty()) {
        QueueItem current = vertexQueue.front();
        vertexQueue.pop();

        TreeVertex* currentVertex = current.vertex;
        int level = current.level;
        int position = current.position;

        if (currentVertex == nullptr) continue;

        // Store the position for this node
        nodePositions[level].push_back(position);

        // Add the value to the correct position
        std::string valueStr = std::to_string(currentVertex->value);
        int startPos = position - valueStr.length() / 2;
        levelLines[level][0].replace(startPos, valueStr.length(), valueStr);

        // Calculate positions for children
        int nextLevelGap = (1 << (depth - level - 2));
        if (level + 1 < depth) {
            int leftPos = position - nextLevelGap;
            int rightPos = position + nextLevelGap;

            // Add left child
            if (currentVertex->leftChild != nullptr) {
                vertexQueue.push(QueueItem(currentVertex->leftChild, level + 1, leftPos));

                // Draw connection with underscores to left child
                for (int i = position - 1; i > leftPos; i--) {
                    if (levelLines[level][0][i] == ' ') {
                        levelLines[level][0][i] = '_';
                    }
                }
            }

            // Add right child
            if (currentVertex->rightChild != nullptr) {
                vertexQueue.push(QueueItem(currentVertex->rightChild, level + 1, rightPos));

                // Draw connection with underscores to right child
                for (int i = position + 1; i < rightPos; i++) {
                    if (levelLines[level][0][i] == ' ') {
                        levelLines[level][0][i] = '_';
                    }
                }
            }
        }
    }

    // Print the tree level by level
    for (int i = 0; i < depth; i++) {
        std::cout << levelLines[i][0] << std::endl;
    }

    std::cout << std::string(maxWidth / 2, '-') << std::endl;
}