// reader.cpp
// AUTHOR: Violet McClure

#include <iostream>
#include <stack>
#include <queue>
#include <vector>
#include <string>
#include <regex>

#include <sstream>

#include "reader.h"

reader::reader() {}

std::vector<std::string> reader::readEq() {
    std::cout << "ACCEPTABLE NOTATION (where n is a constant):\n\tn*sin(x)\n\tdon't use nsin(x)" << std::endl;
    std::cout << "\e[34;1mEnter an equation: \e[0m";

    std::string line;
    std::cin.clear();
    getline(std::cin, line);
    std::stringstream ss(line);

    std::vector<std::string> tokens;

    std::string full = "", word;
    ss >> std::ws;
    while (ss >> word) {
        tokens.push_back(word);
    }
    ss >> std::skipws;

    return tokens;
}

std::vector<std::string> reader::revPol(std::vector<std::string> eq) {
    std::vector<std::string> tokens; // tokenized eq

    std::string token = "", word;
    char ch;
    // read tokens including delimeters ' ', '+', '-', '*', '/', '^', '(', ')'...... and 'x'
    for (unsigned int i = 0; i < eq.size(); i++) { // run through each whitespace delimited word
        word = eq[i];

        for (unsigned int j = 0; j < word.length(); j++) { // run through each character of each word
            ch = word[j];
            std::cout << "ch: " << ch << std::endl;
            if (isOperator(ch) || ch == '(' || ch == ')') {
                if (token != "") {
                    tokens.push_back(token);
                    token = "";
                }
                if (ch == '(') {
                    if (!tokens.empty() && isNumber(tokens.back())) {
                        tokens.push_back("*");
                    }
                }
                token += ch;
                tokens.push_back(token);
                token = "";
            } else if (ch == 'x' || ch == 'X') {
                if (token != "") {
                    tokens.push_back(token);
                    if (!isOperator(token)) {
                        tokens.push_back("*");
                    }
                    token = "";
                }
                tokens.push_back("x");
            } else {
                token += ch;
            }
        }
        if (token != "") {
            tokens.push_back(token);
            token = "";
        }
    }

    std::vector<std::string> out;   // output "queue"
    std::stack<std::string> ops;    // operator stack

    for (unsigned int i = 0; i < tokens.size(); i++) {
        token = tokens[i];
        std::cout << "token: " << token << std::endl;
        if (isNumber(token) || token == "x") {
            out.push_back(token);
        } else if (token == "(") {
            ops.push(token);
        } else if (token == ")") {
            while (! ops.empty() && ops.top() != "(") {
                out.push_back(ops.top());
                ops.pop();
            }
            if (!ops.empty()) {
                ops.pop();
            }
        } else if (isOperator(token) || isTrig(token)) {
            while (! ops.empty() && priority(ops.top()) > priority(token)) {
                out.push_back(ops.top());
                ops.pop();
            }
            ops.push(token);
        }
    }

    while (! ops.empty()) {
        out.push_back(ops.top());
        ops.pop();
    }

    return out;
}

int reader::priority(std::string a) {
    if (a == "+" || a == "-")                       return 1;
    else if (a == "*" || a == "/")                  return 2;
    else if (a == "^" || isTrig(a))                 return 3;
    else                                            return 10;
}

bool reader::isOperator(std::string a) {
    return (a == "+" || a == "-" || a == "*" || a == "/" || a == "^");
}

bool reader::isOperator(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^');
}

bool reader::isTrig(std::string a) {
    return (a == "sin" || a == "cos" || a == "tan" || a == "csc" || a == "sec" || a == "cot");
}

bool reader::isNumber(std::string s) {
    char ch;
    bool dotHook = false;
    for (unsigned int i = 0; i < s.size(); i++) {
        ch = s[i];
        if ((ch < '0' || ch > '9')) {
            if (ch == '.') {
                if (!dotHook) {
                    dotHook = true;
                    continue;
                }
            }
            return false;
        }
    }
    return true;
}

