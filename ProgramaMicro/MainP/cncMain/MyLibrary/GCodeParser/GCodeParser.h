#ifndef GCODEPARSER_H
#define GCODEPARSER_H

#include <sstream>
#include <string>
#include <iomanip>
#include <cstring>
#include <cstdio>
#include <stdio.h>

class GCodeParser{
    public:
    GCodeParser();
    void extractGCodeComponents(const std::string& gcode, double &xValue, double &yValue, double &zValue, double &iValue, double &jValue, double &rValue);
    void doubleToCharArray(double value, char* buffer);
};

#endif // GCODEPARSER_H