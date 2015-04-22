// grapher.h
// AUTHOR: Violet McClure

#ifndef GRAPHER_H
#define GRAPHER_H

#include <vector>
#include <string>

class grapher {
    private:
    int xwin, ywin;                 // starting coordinates of the window for the actual graph
    int w, h;                       // width, height of the window for the actual graph
    double xmin, xmax, ymin, ymax;  // min/max values for x/y for theoretical graph

    std::vector<float> xpts, ypts;    // x, y points that get graphed

    public:
    grapher();
    void graph(std::vector<std::string>);               // actually raises xwindow and displays graph
    void evaluateRevPol(std::vector<std::string>);      // calculates xpts, ypts from Reverse Polish Notation
    bool isOperator(std::string);
    bool isTrig(std::string);
    bool isNumber(std::string);
};

#endif

