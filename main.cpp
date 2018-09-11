#define CATCH_CONFIG_RUNNER
#include <iostream>
#include "userinterface.h"
#include "catch.hpp"
using namespace std;
int main(int argc, char * argv[])
{
    /**
     * @brief A: this is the catalyst for the search engine
     *        prompts the user interface and takes user input
     * TO SHOW DOXYGEN
     *      -Go to Documentation/html/index.html
     *      -Doxygen  as bug where it does not read queryprocessor class
     */
    if(argc > 1 && strcmp(argv[1],"-t") == 0) {
        return Catch::Session().run();
    }
    else {
        userInterface A;
        A.displayMenu();
    }
    return 0;

}
