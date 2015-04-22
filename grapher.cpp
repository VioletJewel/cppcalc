// grapher.cpp
// AUTHOR: Violet McClure

#include <X11/Xlib.h> // "sudo apt-get install libx11-dev"

#include <vector>
#include <stack>

#include <math.h>
#include <string>
#include <cstring>

#include <iostream>
#include <exception>

#include "grapher.h"

grapher::grapher() {
    this->xwin = 50;
    this->ywin = 50;
    this->w = 800; // width
    this->h = 600; // height
    this->xmin = -10.0;
    this->xmax = 10.0;
    this->ymin = -10.0;
    this->ymax = 10.0;
    //this->ymax = this->ymin + (this->h / 2.0) / (this->w / (this->xmin - this->xmax)); // gives you square graphing mode

    //this->xpts ==> std::vector<int>
    //this->ypts ==> std::vector<int>
}

void grapher::graph(std::vector<std::string> tokens) {
    evaluateRevPol(tokens); // generates xpts, ypts

    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 500L; // 500 nano seconds

    Display *d; // display
    Window w; // window
    XEvent e; // event (keypress, etc)

    const char *msg = "AUTHOR: Violet McClure"; // message to be outputted to screen
    int s; // default screen

    d = XOpenDisplay(NULL);
    if (d == NULL) {
       std::cerr << "Cannot open display " << std::endl;
       return;
    }
    
    s = DefaultScreen(d);
    w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, 1000, 1000, 1, BlackPixel(d, s), WhitePixel(d, s)); // create the window
    XSelectInput(d, w, ExposureMask | KeyPressMask); // ?? set up input mask I believe
    XMapWindow(d, w); // finalize window

    while (true) { // event loop
        XNextEvent(d, &e); // repoulate events (pretty sure this blocks)
        if (e.type == Expose) { // first screen (first frame before redraw)
            double x, y = this->ywin;
            for (x = this->xwin - 4; x < this->xwin + this->w; x++) {
                XFillRectangle(d, w, DefaultGC(d, s), x, y - 4, 4, 4);
                XFillRectangle(d, w, DefaultGC(d, s), x, y + this->h, 4, 4);
            }
            x = xwin;
            for (y = this->ywin - 4; y < this->ywin + this->h; y++) {
                XFillRectangle(d, w, DefaultGC(d, s), x - 4, y, 4, 4);
                XFillRectangle(d, w, DefaultGC(d, s), x + this->w, y, 4, 4);
            }
            y = this->ywin + this->h * (this->ymax - 0) / (this->ymax - this->ymin);
            for (x = this->xwin; x < this->xwin + this->w; x++) {   // display x-axis
                XFillRectangle(d, w, DefaultGC(d, s), x, y, 1, 1);
            }
            for (int i = (int)(this->xmin - .5); i <= (int)(this->xmax + 6); i++) { // display tick-marks
                for (int j = 0; j < 5; j++) {
                    XFillRectangle(d, w, DefaultGC(d, s), this->xwin + this->h * (i - this->xmin) / (this->xmax - this->xmin), y - j, 1, 1);
                }
            }
            x = this->xwin + this->w * (0 - this->xmin) / (this->xmax - this->xmin);
            for (y = this->ywin; y < this->ywin + this->h; y++) {   //display y-axis
                XFillRectangle(d, w, DefaultGC(d, s), x, y, 1, 1);
            }
            for (int i = (int)(this->ymin - .5); i < (int)(this->ymax + .5); i++) { // display tick-marks
                for (int j = 0; j < 5; j++) {
                    XFillRectangle(d, w, DefaultGC(d, s), x + j, this->ywin + this->h * (this->ymax - i) / (this->ymax - this->ymin), 1, 1);
                }
            }
        }
        for (int i = 0; i < this->w; i++) {
            if (this->ypts[i] < ywin || this->ypts[i] > ywin + h) {
                continue;
            }
            XFillRectangle(d, w, DefaultGC(d, s), this->xpts[i], this->ypts[i], 2, 2);
        }
        XDrawString(d, w, DefaultGC(d,s), this->xwin + 15, this->ywin + this->h  + 20, msg, strlen(msg));
        if (e.type == KeyPress) { // if event is a keypress
            //std::cout << e.xkey.keycode << std::endl; // simply print keycode to screen
        }
        if (e.xkey.keycode  == 24) { // if keycode is 'q'
            break;
        }
        nanosleep(&ts, NULL); // should probably be a really small sleep here!!
    }
    XCloseDisplay(d); // properly close window/display
}

void grapher::evaluateRevPol(std::vector<std::string> tokens) {
    std::stack<std::string> stack;
    std::string token;
    double d1, d2, result;
    double x = xmin;   // x point
    std::string::size_type sz;

    this->xpts.clear();
    this->ypts.clear();

    double delta = (this->xmax - this->xmin) / this->w;

    for (int i = 0; i < this->w; i++, x += delta) {
        for (unsigned int j = 0; j < tokens.size(); j++) {
            token = tokens[j];

            if (token == "x") {
                token = std::to_string(x);
            }

            if (!isOperator(token) && !isTrig(token)) {
                stack.push(token);
            } else {
                if (isTrig(token)) {
                    d1 = std::stod(stack.top(), &sz); stack.pop();
                    result = (token == "sin") ? sin(d1) :/*else*/ (token == "cos") ? cos(d1) :/*else*/ (token == "tan") ? tan(d1) :/*else*/ (token == "csc") ? 1.0/sin(d1) :/*else*/ (token == "sec") ? 1.0/cos(d1) :/*else (token == "cot") ?*/ 1.0/tan(d1);
                } else {
                    d2 = std::stod(stack.top(), &sz); stack.pop();
                    d1 = std::stod(stack.top(), &sz); stack.pop();

                    result = (token == "+") ? (d1 + d2) :/*else*/ (token == "-") ? d1 - d2 :/*else*/ (token == "*") ? (d1 * d2) :/*else*/ (token == "/") ? d1 / d2 :/*else token must be power*/ pow(d1, d2);
                }
                
                stack.push(std::to_string(result));
            }
        }

        this->xpts.push_back(this->xwin + i);
        this->ypts.push_back(this->ywin + this->h * (this->ymax - std::stod(stack.top(), &sz)) / (this->ymax - this->ymin));
    }
}

bool grapher::isOperator(std::string a) {
    return (a == "+" || a == "-" || a == "*" || a == "/" || a == "^" || isTrig(a));
}

bool grapher::isTrig(std::string a) {
    return (a == "sin" || a == "cos" || a == "tan" || a == "csc" || a == "sec" || a == "cot");
}

bool grapher::isNumber(std::string s) {
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

