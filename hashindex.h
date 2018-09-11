#ifndef HASHINDEX_H
#define HASHINDEX_H
#include "indexinterface.h"
#include "reader.h"
#include "queryprocessor.h"
#include "utility"
/**
 * @brief The hashIndex class : this class uses all the methods from indexInterface and uses
 * Hash_Map implementation for our crucial members
 */
class hashIndex : public indexInterface
{
private:
    std::string tops50; //top 50 words
    int totalQ= 0;      //# of questions
    int totalWord = 0;  //total number of words
    reader reads;
    vector<string> allKey;  //each key value
    vector<pair<string,int>> top50;     //top 50 words corresponding to how often they appear
    hash_map<int,hash_map<string,double>> occurs;//holds all unqiue ids plus words w/IDF scores
    hash_map<string,double> TFscores;           //Holds all tf scores for every word
     //this takes all of our seperate data members and combines them to write to our index file
    hash_map<std::string,std::vector<std::pair<int, double>>> fileC;
    hash_map<std::string,hash_map<int,int>> tgID;
    //for loading into query
    hash_map<int,pair<double,bool>> total;
    hash_map<int,double> final;
    vector<int> rests;
public:
    hashIndex();
    void getTF();
    void getIDF();
    void join();
    void writeFile();
    void evaluate(string);
    void adjustStats(int&,int&,string&);
    vector<int> getRests();
};

#endif // HASHINDEX_H
