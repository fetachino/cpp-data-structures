#ifndef DIMENSIONAL_H
#define DIMENSIONAL_H
#include <string>

/*
* Multilist Structure for Student-Class Data:
* - First row: Student list indexing student numbers (1~10000)
* - First column: Class list indexing class numbers (1~3000)
* - Each node stores: {student number, class number, grade}
* - Grade range: 0~100 (can be empty/unset initially)
* - Nodes are created only when a student-class pair is input
*/

// Value ranges for validation
const int MIN_STUDENT_ID = 1;
const int MAX_STUDENT_ID = 10000;
const int MIN_CLASS_ID = 1;
const int MAX_CLASS_ID = 3000;
const int MIN_GRADE = 0;
const int MAX_GRADE = 100;
const int GRADE_UNSET = -1;  // Indicates grade hasn't been input yet

// Node structure for the multilist
struct Node {
    int studentID;     // Range: 1~10000
    int classID;       // Range: 1~3000
    int grade;         // Range: 0~100, or -1 if not yet set
    Node* nextStudent; // Horizontal pointer - links nodes in student list
    Node* nextClass;   // Vertical pointer - links nodes in class list

    // Constructor: grade defaults to -1 (unset) until explicitly provided
    Node(int sid = 0, int cid = 0, int g = GRADE_UNSET);
};

class Dimensional {
private:
    Node* studentHead;  // Head of student index list (first row)
    Node* classHead;    // Head of class index list (first column)

    // Helper functions for node management
    Node* findOrCreateStudentIndex(int studentID);  // Handles student index list
    Node* findOrCreateClassIndex(int classID);      // Handles class index list
    Node* findNode(int studentID, int classID) const;  // Searches for existing node

public:
    Dimensional();
    ~Dimensional();

    // Core functions as per requirements
    bool insert(int studentID, int classID, int grade = GRADE_UNSET);  // Insert new student-class pair
    bool updateGrade(int studentID, int classID, int grade);  // Update grade after insertion
    void printClassLists() const;    // Print all non-empty class lists
    void printStudentLists() const;  // Print all active student lists
    void readFromFile(const std::string& filename);  // Read data from text file
};

#endif