#include <iostream>
#include <string>
#include <sstream>
#include <cctype>

int main() {
    std::string input = "bytes=900-";
    std::string result;

    // Iterate through each character in the string
    for (std::string::iterator it = input.begin(); it != input.end(); ++it) {
        // Check if the character is a digit
        if (std::isdigit(*it)) {
            result += *it;  // Append the digit to the result string
        }
    }

    // Convert the result string to an integer
    int number;
    std::istringstream(result) >> number;

    // Print the extracted number
    std::cout << "Extracted number: " << number << std::endl;

    return 0;
}
