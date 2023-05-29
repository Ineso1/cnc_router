// #include <iostream>
// #include <string>
// #include <sstream>
// #include <vector>

// // Function to split a string by a delimiter
// std::vector<std::string> splitString(const std::string& input, char delimiter) {
//     std::vector<std::string> result;
//     std::stringstream ss(input);
//     std::string item;

//     while (getline(ss, item, delimiter)) {
//         result.push_back(item);
//     }

//     return result;
// }

// // Function to extract GCode components
// void extractGCodeComponents(const std::string& gcode) {
//     std::vector<std::string> parts = splitString(gcode, ' ');
//     std::string gcodeNumber;
//     std::string xValue, yValue, zValue;
//     std::string iValue, jValue, rValue;

//     for (const std::string& part : parts) {
//         if (part[0] == 'G') {
//             gcodeNumber = part.substr(1);
//         } else if (part[0] == 'X') {
//             xValue = part.substr(1);
//         } else if (part[0] == 'Y') {
//             yValue = part.substr(1);
//         } else if (part[0] == 'Z') {
//             zValue = part.substr(1);
//         } else if (part[0] == 'I') {
//             iValue = part.substr(1);
//         } else if (part[0] == 'J') {
//             jValue = part.substr(1);
//         } else if (part[0] == 'R') {
//             rValue = part.substr(1);
//         }
//     }

//     // Print the extracted components
//     std::cout << "GCode Number: " << gcodeNumber << std::endl;
//     std::cout << "X Value: " << xValue << std::endl;
//     std::cout << "Y Value: " << yValue << std::endl;
//     std::cout << "Z Value: " << zValue << std::endl;
    
//     // For G2 or G3, print the additional components
//     if (gcodeNumber == "2" || gcodeNumber == "3") {
//         std::cout << "I Value: " << iValue << std::endl;
//         std::cout << "J Value: " << jValue << std::endl;
//         std::cout << "R Value: " << rValue << std::endl;
//     }
// }

// int main() {
//     std::string gcode = "G1 X10.5 Y20.7 Z5.3";
//     extractGCodeComponents(gcode);
//     return 0;
// }




// #include <iostream>
// #include <cstring>
// #include <cstdlib>

// // Function to extract GCode components
// void extractGCodeComponents(const char* gcode) {
//     char* token;
//     char* gcodeCopy = strdup(gcode); // Create a copy of the input GCode string

//     // Extract components using strtok function
//     token = strtok(gcodeCopy, " ");
//     char* gcodeNumber = nullptr;
//     double xValue = 0.0;
//     double yValue = 0.0;
//     double zValue = 0.0;
//     double iValue = 0.0;
//     double jValue = 0.0;
//     double rValue = 0.0;

//     while (token != nullptr) {
//         if (token[0] == 'G') {
//             gcodeNumber = &token[1];
//         } else if (token[0] == 'X') {
//             xValue = strtod(&token[1], nullptr);
//         } else if (token[0] == 'Y') {
//             yValue = strtod(&token[1], nullptr);
//         } else if (token[0] == 'Z') {
//             zValue = strtod(&token[1], nullptr);
//         } else if (token[0] == 'I') {
//             iValue = strtod(&token[1], nullptr);
//         } else if (token[0] == 'J') {
//             jValue = strtod(&token[1], nullptr);
//         } else if (token[0] == 'R') {
//             rValue = strtod(&token[1], nullptr);
//         }

//         token = strtok(nullptr, " ");
//     }

//     // Print the extracted components
//     std::cout << "GCode Number: " << gcodeNumber << std::endl;
//     std::cout << "X Value: " << xValue << std::endl;
//     std::cout << "Y Value: " << yValue << std::endl;
//     std::cout << "Z Value: " << zValue << std::endl;

//     // For G2 or G3, print the additional components
//     if (gcodeNumber != nullptr && (strcmp(gcodeNumber, "2") == 0 || strcmp(gcodeNumber, "3") == 0)) {
//         std::cout << "I Value: " << iValue << std::endl;
//         std::cout << "J Value: " << jValue << std::endl;
//         std::cout << "R Value: " << rValue << std::endl;
//     }

//     free(gcodeCopy); // Free the dynamically allocated memory
// }

// int main() {
//     const char* gcode = "G1 X-1000.5123456789 Y20.7123456789 Z-5.3123456789";
//     extractGCodeComponents(gcode);
//     return 0;
// }



#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <string.h>

// Function to extract GCode components
void extractGCodeComponents(const std::string& gcode) {
    std::istringstream iss(gcode);
    std::string token;
    std::string gcodeNumber;
    double xValue = 0.0;
    double yValue = 0.0;
    double zValue = 0.0;
    double iValue = 0.0;
    double jValue = 0.0;
    double rValue = 0.0;

    while (iss >> token) {
        if (token[0] == 'G') {
            gcodeNumber = token.substr(1);
        } else if (token[0] == 'X') {
            xValue = std::stod(token.substr(1));
        } else if (token[0] == 'Y') {
            yValue = std::stod(token.substr(1));
        } else if (token[0] == 'Z') {
            zValue = std::stod(token.substr(1));
        } else if (token[0] == 'I') {
            iValue = std::stod(token.substr(1));
        } else if (token[0] == 'J') {
            jValue = std::stod(token.substr(1));
        } else if (token[0] == 'R') {
            rValue = std::stod(token.substr(1));
        }
    }

    // Print the extracted components
    std::cout << std::fixed << std::setprecision(10); // Set the precision for floating-point output
    std::cout << "GCode Number: " << gcodeNumber << std::endl;
    std::cout << "X Value: " << xValue << std::endl;
    std::cout << "Y Value: " << yValue << std::endl;
    std::cout << "Z Value: " << zValue << std::endl;

    // For G2 or G3, print the additional components
    if (!gcodeNumber.empty() && (gcodeNumber == "2" || gcodeNumber == "3")) {
        std::cout << "I Value: " << iValue << std::endl;
        std::cout << "J Value: " << jValue << std::endl;
        std::cout << "R Value: " << rValue << std::endl;
    }

    char buffer[sizeof(xValue)];
    *(double*)buffer = xValue;

    std::cout << "prueba: " << buffer;
}

int main() {
    std::string gcode = "G1 X1234567890.123456789 Y9876543210.987654321 Z12345.67890";
    extractGCodeComponents(gcode);
    return 0;
}
