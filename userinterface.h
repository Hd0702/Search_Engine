#ifndef USERINTERFACE_H
#define USERINTERFACE_H
#include "indexinterface.h"
#include "avlindex.h"
#include "hashindex.h"
#include <cstdio>
/**
 * @brief The userInterface class : User will determine which mode runs,
 * modes have NO overlap, once one is finished project is terminated
 */
class userInterface
{
public:
    userInterface();
    void displayMenu();
private:
    indexInterface * inter;
    void matinance();
    void interactive();

};

#endif // USERINTERFACE_H
