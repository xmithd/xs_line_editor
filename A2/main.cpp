#include <iostream>
#include <string>
#include "LineEditor.h"

using namespace std;

int main(int argc, const char * argv[]) {

    string filename;
    int ret(EXIT_SUCCESS);
    switch (argc) {
        case 1:
            cerr << "No filename given." << endl;
            ret = EXIT_FAILURE;
            break;
        case 2:
            {
                // LineEditor exits in case of failure
                // or when the user inputs the quit command.
                filename = argv[1];
                LineEditor ed(filename);
                ed.run();
            }
            break;
        default:
            cerr << "Too many arguments." << endl;
            ret = EXIT_FAILURE;
            break;
    }

    return ret;
}
