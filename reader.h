// reader.h
// AUTHOR: Violet McClure

#ifndef READER_H
#define READER_H

#include <vector>
#include <string>

class reader {
    public:
    reader();
    std::vector<std::string> readEq();
    std::vector<std::string> revPol(std::vector<std::string>);
    int priority(std::string);
    bool isOperator(std::string);
    bool isOperator(char);
    bool isTrig(std::string);
    bool isNumber(std::string);
};

#endif

