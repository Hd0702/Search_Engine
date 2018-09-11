#include"dstree.h"
#include <string>
#include <fstream>
#include "avlindex.h"
#include "hashindex.h"
#include "hashmap.h"
#include <functional>
#include <algorithm>
#include "reader.h"
#include "boost/algorithm/string.hpp"
#include "porter2_stemmer.h"
#include <utility>
#include "boost/tokenizer.hpp"
#include "indexinterface.h"
#include "boost/lexical_cast.hpp"
#ifndef QUERYPROCESSOR_H
#define QUERYPROCESSOR_H

using namespace  std;
/**
 * @brief The queryProcessor class : this class takes the user input query,
 * splits it and stems it, and then it reads the index file for every word and stores it \n
 * this class is basically an extension of the index interfacep
 */
class queryProcessor
{
public:
    queryProcessor(string);
    void splitQuery();
    dstree<int,pair<double,bool>> total;
    dstree<int,double> final;
    friend class avlIndex;
    friend class hashIndex;
private:
    indexInterface * A;
    pair<string,vector<pair<pair<int,bool>,double>>> readQuery(string);
    reader readstop;
    string query;
    ifstream reads;
    vector<string> ands;
    vector<string> ors;
    vector<int> Ids;
    dstree<int,int> and_Counter;
    vector<string> nots;
    vector<pair<int,double>> save_this;
    /**
    THese 3 vectors may look complicated, it is essential a word key value plus a vector where ...
    the first int is the ID number, a bool if the int has been repeated (for ANDS)
    and a double for the TFIDF score
    **/
    vector<pair<string,vector<pair<pair<int,bool>,double>>>> and_vect;
    vector<pair<string,vector<pair<pair<int,bool>,double>>>> or_vect;
    vector<pair<string,vector<pair<pair<int,bool>,double>>>> not_vect;
};

#endif // QUERYPROCESSOR_H
