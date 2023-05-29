#include "GCodeParser.h"

GCodeParser::GCodeParser(){}

// Function to extract GCode components
void GCodeParser::extractGCodeComponents(const std::string& gcode, double &xValue, double &yValue, double &zValue, double &iValue, double &jValue, double &rValue) {
    std::istringstream iss(gcode);
    std::string token;
    std::string gcodeNumber;
    xValue = 0.0;
    yValue = 0.0;
    zValue = 0.0;
    iValue = 0.0;
    jValue = 0.0;
    rValue = 0.0;

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
}

// Function to convert a double to a char array
void doubleToCharArray(double value, char* buffer) {
    std::string s{std::to_string(value)};
    int i = 0;
    for (const auto& c : s)
    {
        buffer[i] = c;
        i++;
    }
}



