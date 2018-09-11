#ifndef AVLINDEX_H
#define AVLINDEX_H
#include "indexinterface.h"
#include "reader.h"
#include "utility"
#include "queryprocessor.h"
/**
 * @brief The avlIndex class : this class uses all the methods from indexInterface and uses
 * AVLtree implementation for our crucial members
 */
class avlIndex: public indexInterface
{
private:
    std::string tops50;  //top 50 words
    int totalQ = 0;     //# of questions
    int totalWord = 0;  //total amount of words
    string look;
    reader reads;       //this is used for private read members from reading/parsing data
    dstree<int,dstree<string,double>> occurs;  //holds all unqiue ids plus words w/IDF scores
    dstree<string,double> TFscores;             //Holds all tf scores for every word
    vector<pair<string,int>> top50;             //holds top 50 members for index stats
    vector<string> allKey;                      //holds every individual unique word
    //this takes all of our seperate data members and combines them to write to our index file
    dstree<std::string,std::vector<std::pair<int, double>>> fileC;
    dstree<std::string,dstree<int,int>> tgID;  //each unique id for a key word
    //for loading into query
    dstree<int,pair<double,bool>> total;        //THIS takes all of our TFIDF and adds them together
    dstree<int,double> final;                   //takes total and writes it to file
    vector<int> rests;                      //this holds our ids for our query item

public:
    friend class queryProcessor;
    avlIndex(string a = "-1");
    void getTF();
    void getIDF();
    void join();
    void writeFile();
    void evaluate(string);
    void adjustStats(int&,int&,string&);
    vector<int> getRests();
};

#endif // AVLINDEX_H
