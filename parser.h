#ifndef PARSER_H
#define PARSER_H
//this is our tfidf
#include <cmath>
#include "reader.h"
#include "avlindex.h"
/**
 * @brief The parser class
 * STEP 1: WE NEED THE TOTAL AMOUNT OF TIMES A WORD APPEARS
 *      -Do this in reader by adding to value if word exists, DONt use ID
 *      -add a bool to token function and true if for tags, so we dont add to Tfidf
 * STEP 2: We must go through each Question Title, check how many times each word appears
 *          and compare it to the total amount using formula
 *         - go through each vector of titiles and add it to AVL tree
 * STEP 3: assign formula to each individual value and replace our value with it
 * STEP 4: do this foe every question
 * STEP 5: (temp step) write every value to the file with word, id#, and score
 * STEP 6: check scores somehow and set a normal score for the word to be included to the id#
 * maybe we could just print the scores in the index interface
 */
using namespace std;
class parser
{
public:
    parser();
    friend class avlIndex;
private:
    reader reads;
    void checkQs();
    void getTF();
    void getIDF();
    dstree<int,dstree<string,double>> occurs;
    dstree<string,double> TFscores;
    //adding above varaible causes segfault?
};

#endif // PARSER_H
