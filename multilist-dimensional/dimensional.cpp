#include "dimensional.h"
#include "dimensional.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>

Node::Node(int sid, int cid, int g) :
    studentID(sid), classID(cid), grade(g),
    nextStudent(nullptr), nextClass(nullptr) {
}

Dimensional::Dimensional() : studentHead(nullptr), classHead(nullptr) {}

Dimensional::~Dimensional() {
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

Node* Dimensional::findOrCreateStudentIndex(int studentID) {
    if (!studentHead) {
        studentHead = new Node(studentID, 0);
        return studentHead;
    }

    Node* prev = nullptr;
    Node* curr = studentHead;

    while (curr && curr->studentID < studentID) {
        prev = curr;
        curr = curr->nextStudent;
    }

    if (curr && curr->studentID == studentID) {
        return curr;
    }

    Node* newIndex = new Node(studentID, 0);
    newIndex->nextStudent = curr;
    if (prev) {
        prev->nextStudent = newIndex;
    }
    else {
        studentHead = newIndex;
    }

    return newIndex;
}

Node* Dimensional::findOrCreateClassIndex(int classID) {
    if (!classHead) {
        classHead = new Node(0, classID);
        return classHead;
    }

    Node* prev = nullptr;
    Node* curr = classHead;

    while (curr && curr->classID < classID) {
        prev = curr;
        curr = curr->nextClass;
    }

    if (curr && curr->classID == classID) {
        return curr;
    }

    Node* newIndex = new Node(0, classID);
    newIndex->nextClass = curr;
    if (prev) {
        prev->nextClass = newIndex;
    }
    else {
        classHead = newIndex;
    }

    return newIndex;
}

Node* Dimensional::findNode(int studentID, int classID) const {
    Node* studentIndex = studentHead;
    while (studentIndex && studentIndex->studentID < studentID) {
        studentIndex = studentIndex->nextStudent;
    }
    if (!studentIndex || studentIndex->studentID != studentID) {
        return nullptr;
    }

    Node* curr = studentIndex->nextClass;
    while (curr && curr->classID < classID) {
        curr = curr->nextClass;
    }
    if (curr && curr->classID == classID) {
        return curr;
    }
    return nullptr;
}

bool Dimensional::insert(int studentID, int classID, int grade) {
    if (studentID < MIN_STUDENT_ID || studentID > MAX_STUDENT_ID ||
        classID < MIN_CLASS_ID || classID > MAX_CLASS_ID ||
        (grade != GRADE_UNSET && (grade < MIN_GRADE || grade > MAX_GRADE))) {
        std::cout << "Invalid input ranges!" << std::endl;
        return false;
    }

    if (findNode(studentID, classID)) {
        std::cout << "Entry already exists!" << std::endl;
        return false;
    }

    Node* newNode = new Node(studentID, classID, grade);
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

void Dimensional::printClassLists() const {
    std::cout << "\nClass Lists:" << std::endl;
    Node* classPtr = classHead;
    while (classPtr) {
        if (classPtr->classID != 0) {
            std::cout << "Class " << classPtr->classID << ": Students ";
            Node* studentPtr = classPtr->nextStudent;
            bool first = true;
            while (studentPtr) {
                if (!first) std::cout << ", ";
                std::cout << studentPtr->studentID;
                if (studentPtr->grade != GRADE_UNSET) {
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

void Dimensional::printStudentLists() const {
    std::cout << "\nStudent Lists:" << std::endl;
    Node* studentPtr = studentHead;
    while (studentPtr) {
        if (studentPtr->studentID != 0) {
            std::cout << "Student " << studentPtr->studentID << ": Classes ";
            Node* classPtr = studentPtr->nextClass;
            bool first = true;
            while (classPtr) {
                if (!first) std::cout << ", ";
                std::cout << classPtr->classID;
                if (classPtr->grade != GRADE_UNSET) {
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

bool Dimensional::updateGrade(int studentID, int classID, int grade) {
    if (grade < MIN_GRADE || grade > MAX_GRADE) {
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

void Dimensional::readFromFile(const std::string& filename) {
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
                    values.size() > 2 ? values[2] : GRADE_UNSET);
            }
        }
    }
    inputFile.close();
}