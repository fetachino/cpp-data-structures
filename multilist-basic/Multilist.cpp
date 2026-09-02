#include "Multilist.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

Node::Node(int sid, int cid, int g) :
    studentID(sid), classID(cid), grade(g),
    nextStudent(nullptr), nextClass(nullptr) {
}

Multilist::Multilist() : studentHead(nullptr), classHead(nullptr) {}

Multilist::~Multilist() {
    // Delete all non-index nodes
    Node* sPtr = studentHead;
    while (sPtr) {
        Node* cPtr = sPtr->nextClass;
        while (cPtr) {
            Node* temp = cPtr;
            cPtr = cPtr->nextClass;
            delete temp;
        }
        Node* temp = sPtr;
        sPtr = sPtr->nextStudent;
        delete temp;
    }
}

Node* Multilist::findOrCreateStudentIndex(int studentID) {
    // If student index list is empty, create first index node
    if (!studentHead) {
        studentHead = new Node(studentID, 0);  // Use 0 for dummy class field
        return studentHead;
    }

    Node* prev = nullptr;
    Node* curr = studentHead;

    // Find correct position in sorted student index list
    while (curr && curr->studentID < studentID) {
        prev = curr;
        curr = curr->nextStudent;
    }

    // If student index already exists
    if (curr && curr->studentID == studentID) {
        return curr;
    }

    // Create new student index node
    Node* newIndex = new Node(studentID, 0);  // Use 0 for dummy class field
    newIndex->nextStudent = curr;
    if (prev) {
        prev->nextStudent = newIndex;
    }
    else {
        studentHead = newIndex;
    }

    return newIndex;
}

Node* Multilist::findOrCreateClassIndex(int classID) {
    // If class index list is empty, create first index node
    if (!classHead) {
        classHead = new Node(0, classID);  // Use 0 for dummy student field
        return classHead;
    }

    Node* prev = nullptr;
    Node* curr = classHead;

    // Find correct position in sorted class index list
    while (curr && curr->classID < classID) {
        prev = curr;
        curr = curr->nextClass;
    }

    // If class index already exists
    if (curr && curr->classID == classID) {
        return curr;
    }

    // Create new class index node
    Node* newIndex = new Node(0, classID);  // Use 0 for dummy student field
    newIndex->nextClass = curr;
    if (prev) {
        prev->nextClass = newIndex;
    }
    else {
        classHead = newIndex;
    }

    return newIndex;
}

Node* Multilist::findNode(int studentID, int classID) const {
    // Find student index
    Node* studentIndex = studentHead;
    while (studentIndex && studentIndex->studentID < studentID) {
        studentIndex = studentIndex->nextStudent;
    }
    if (!studentIndex || studentIndex->studentID != studentID) {
        return nullptr;
    }

    // Search through student's class list
    Node* curr = studentIndex->nextClass;
    while (curr && curr->classID < classID) {
        curr = curr->nextClass;
    }
    if (curr && curr->classID == classID) {
        return curr;
    }
    return nullptr;
}

bool Multilist::insert(int studentID, int classID, int grade) {
    // Validate input ranges
    if (studentID < 1 || studentID > 10000 ||
        classID < 1 || classID > 3000 ||
        (grade != -1 && (grade < 0 || grade > 100))) {
        std::cout << "Invalid input ranges!" << std::endl;
        return false;
    }

    // Check if node already exists
    if (findNode(studentID, classID)) {
        std::cout << "Entry already exists!" << std::endl;
        return false;
    }

    // Create new node
    Node* newNode = new Node(studentID, classID, grade);

    // Get or create index nodes
    Node* studentIndex = findOrCreateStudentIndex(studentID);
    Node* classIndex = findOrCreateClassIndex(classID);

    // Insert into student's class list
    Node* curr = studentIndex->nextClass;
    Node* prev = nullptr;
    while (curr && curr->classID < classID) {
        prev = curr;
        curr = curr->nextClass;
    }
    if (prev) {
        prev->nextClass = newNode;
    }
    else {
        studentIndex->nextClass = newNode;
    }
    newNode->nextClass = curr;

    // Insert into class's student list
    curr = classIndex->nextStudent;
    prev = nullptr;
    while (curr && curr->studentID < studentID) {
        prev = curr;
        curr = curr->nextStudent;
    }
    if (prev) {
        prev->nextStudent = newNode;
    }
    else {
        classIndex->nextStudent = newNode;
    }
    newNode->nextStudent = curr;

    return true;
}

void Multilist::printClassLists() const {
    std::cout << "\nClass Lists:" << std::endl;
    Node* classPtr = classHead;
    while (classPtr) {
        if (classPtr->classID != 0) {  // Skip dummy nodes
            std::cout << "Class " << classPtr->classID << ": Students ";
            Node* studentPtr = classPtr->nextStudent;
            bool first = true;
            while (studentPtr) {
                if (!first) std::cout << ", ";
                std::cout << studentPtr->studentID;
                if (studentPtr->grade != -1) {
                    std::cout << "(" << studentPtr->grade << ")";
                }
                first = false;
                studentPtr = studentPtr->nextStudent;
            }
            std::cout << std::endl;
        }
        classPtr = classPtr->nextClass;
    }
}

void Multilist::printStudentLists() const {
    std::cout << "\nStudent Lists:" << std::endl;
    Node* studentPtr = studentHead;
    while (studentPtr) {
        if (studentPtr->studentID != 0) {  // Skip dummy nodes
            std::cout << "Student " << studentPtr->studentID << ": Classes ";
            Node* classPtr = studentPtr->nextClass;
            bool first = true;
            while (classPtr) {
                if (!first) std::cout << ", ";
                std::cout << classPtr->classID;
                if (classPtr->grade != -1) {
                    std::cout << "(" << classPtr->grade << ")";
                }
                first = false;
                classPtr = classPtr->nextClass;
            }
            std::cout << std::endl;
        }
        studentPtr = studentPtr->nextStudent;
    }
}

bool Multilist::updateGrade(int studentID, int classID, int grade) {
    if (grade < 0 || grade > 100) {
        std::cout << "Invalid grade range!" << std::endl;
        return false;
    }

    Node* node = findNode(studentID, classID);
    if (!node) {
        std::cout << "Entry not found!" << std::endl;
        return false;
    }

    node->grade = grade;
    return true;
}

void Multilist::readFromFile(const std::string& filename) {
    std::ifstream inputFile(filename);
    std::string line;
    while (std::getline(inputFile, line)) {
        line.erase(std::remove(line.begin(), line.end(), '('), line.end());
        line.erase(std::remove(line.begin(), line.end(), ')'), line.end());

        std::stringstream lineStream(line);
        std::string token;
        while (std::getline(lineStream, token, ' ')) {
            if (token.empty()) continue;

            std::stringstream dataStream(token);
            std::string number;
            std::vector<int> values;

            while (std::getline(dataStream, number, ',')) {
                if (!number.empty()) {
                    values.push_back(std::stoi(number));
                }
            }

            if (values.size() >= 2) {
                insert(values[0], values[1],
                    values.size() > 2 ? values[2] : -1);
            }
        }
    }
    inputFile.close();
}