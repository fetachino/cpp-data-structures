#include "dimensional.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

int main() {
    Dimensional dim;
    std::string input;

    std::cout << "Enter student-class pairs or commands:\n";
    std::cout << "Format: (student_id,class_id) or (student_id,class_id,grade)\n";
    std::cout << "Commands: 'file' to read from file, 'print' to show lists, Enter to finish\n";

    while (std::getline(std::cin, input)) {
        if (input == "file") {
            std::cout << "Enter filename: ";
            std::string filename;
            std::getline(std::cin, filename);
            dim.readFromFile(filename);
        }
        else if (input == "print") {
            dim.printClassLists();
            dim.printStudentLists();
        }
        else if (input.empty()) {
            break;
        }
        else {
            input.erase(std::remove(input.begin(), input.end(), '('), input.end());
            input.erase(std::remove(input.begin(), input.end(), ')'), input.end());

            std::stringstream ss(input);
            std::string number;
            std::vector<int> values;

            while (std::getline(ss, number, ',')) {
                if (!number.empty()) {
                    values.push_back(std::stoi(number));
                }
            }

            if (values.size() >= 2) {
                dim.insert(values[0], values[1],
                    values.size() > 2 ? values[2] : GRADE_UNSET);
            }
        }
    }

    dim.printClassLists();
    dim.printStudentLists();

    return 0;
}