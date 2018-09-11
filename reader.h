#ifndef READER_H
#define READER_H
#include <dstree.h>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include "boost/serialization/vector.hpp"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/string.hpp>
#include "porter2_stemmer.h"
#include "boost/tokenizer.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/range/istream_range.hpp"
#include "hashmap.h"
#include "boost/filesystem.hpp"
/**
 * @brief  This class handles all the parsing and storing
     * \n We will used this stored data to eventually create the index file
 */
class reader
{
public:
    reader();
    friend class avlIndex;
    friend class boost::serialization::access;
    friend class hashIndex;
    friend class userInterface;
     void readsTagsNames();
     void readsTags();
     void stopWords();
     void readQuestion(int, bool,string);
     void readQuestionNames(int);
     void addQ(string);
     /**
      * @brief stops : this is used throughout the program to remove stop words from user input
      */
     hash_map<string,int> stops;
private:
     /**
     * @brief unqID: this map contains each unqiue ID value for proper indexing
     * @brief tTags : This holds all the tags for a specific ID value
     * @brief rData:  This holds every single word and how often it appears, used in TF score
     * @brief titles: Holds 2d vector with every single question parsed and stemmed
     * @brief write: Writes titles to serialization archive file
     * @brief tokens: Tokenizes/stems every word in a vector and stores it into titles
     * @brief readNShow : This load serialization file into titles
     */
    std::ifstream reads;
    int count = 0;
    int totalQs =0;
    dstree<int,vector<string>> unqId;
    dstree<int,std::vector<std::string>> tTags;
    dstree<string,int> rawData;        //this is individual words and each time they appear in total
    std::vector<int> tagCheck; //for error checking
    std::vector<std::vector<std::string>> uniques;
    std::vector<std::string> Tags;
    std::vector<std::string> Qs;
    std::vector<std::string> rData;
    std::vector<std::vector<std::string>> titles; //vector of pointers should be faster
    int totalsize =0;
    std::vector<int> IDs;
    inline std::vector<std::string>  tokens(std::string );
    void write();
    void readNShow();
};

#endif // READER_H
