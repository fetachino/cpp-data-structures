#include "Multilist.h"
#include <iostream>
#include <string>
#include <sstream>

int main() {
    Multilist ml;
    std::string input;

    std::cout << "Enter student-class pairs or commands:\n";
    std::cout << "Format: (student_id,class_id) or (student_id,class_id,grade)\n";
    std::cout << "Commands: 'file' to read from file, 'print' to show lists, Enter to finish\n";

    while (std::getline(std::cin, input)) {
        if (input == "file") {
            std::cout << "Enter filename: ";
            std::string filename;
            std::getline(std::cin, filename);
            ml.readFromFile(filename);
        }
        else if (input == "print") {
            ml.printClassLists();
            ml.printStudentLists();
        }
        else if (input.empty()) {
            break;
        }
        else {
            // Parse input
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
                ml.insert(values[0], values[1],
                    values.size() > 2 ? values[2] : -1);
            }
        }
    }

    // Final output
    ml.printClassLists();
    ml.printStudentLists();

    return 0;
}