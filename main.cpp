// main.cpp

#include <iostream>
#include <vector>
#include <string>

#include "reader.h"
#include "grapher.h"

using namespace std;

int main() {
    // AUTHOR: Violet McClure
    reader r; // create a read to read stdin
    vector<string> tokens;
    try {
        tokens = r.readEq(); // read stdin
    } catch (...) {
        cerr << "Unable to read equation" << endl;
        return 1;
    }/*
    for (unsigned int i = 0; i < tokens.size(); i++) {
        cout << "token: " << tokens[i] << endl;
    }
    return 0;*/
    try {
        tokens = r.revPol(tokens);              // convert infix notation to reverse polish notation (can reuse tokens to save memory)
    } catch (...) {
        cerr << "Invalid syntax for equation" << endl;
        return 2;
    }

    grapher g;  // create grapher object
    try {
        g.graph(tokens);   // produce X11 GUI and evaluate each y value from the x values based on the window requirements
    } catch (...) {
        cerr << "Error in graphing window" << endl;
        return 3;
    }
    // AUTHOR: Violet McClure [END]

    return 0;
}

