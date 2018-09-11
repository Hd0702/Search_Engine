#include "queryprocessor.h"
/**
 * @brief queryProcessor::queryProcessor : this takes user input and sets into our processor
 * @param qur : this is the user query we're going to analyze
 */
queryProcessor::queryProcessor(string qur)
{
query = qur;
}
/**
 * @brief queryProcessor::splitQuery : this stems every query and removes all stop words
 */
void queryProcessor::splitQuery() {
    readstop.stopWords();
    //THIS METHOD TAKES OUR QUERY AND SPLITS IT AND STEMS IT
    boost::char_separator<char> sep(" ");
    boost::tokenizer<boost::char_separator<char>> tokens(query,sep);
    boost::tokenizer<boost::char_separator<char>>::iterator iter = tokens.begin();;
    if(*iter == "AND") { //Handles AND words
        ++iter;
        while(iter != tokens.end()) {
            if(*iter != "NOT") {    //go through everything until it hits a NOT and iadd it to and
                string find = *iter;
                boost::algorithm::to_lower(find);
                if(!readstop.stops.contains(find)) {
                    Porter2Stemmer::stem(find);
                    and_vect.push_back(readQuery(find));
                    ands.push_back(find);
                }
                iter++;
            }
            else { //if we found not
                //once we find not we go through the rest of the query and add it to not vector
                iter++;
                while(iter != tokens.end()) { //add all nots
                    string find = *iter;
                    boost::algorithm::to_lower(find);
                    if(!readstop.stops.contains(find)) {
                        Porter2Stemmer::stem(find);
                        not_vect.push_back(readQuery(find));
                        nots.push_back(find);
                    }
                    iter++;
                }
                return;
            }
        }
    }
    else if(*iter == "OR") { //Handles OR words
        ++iter;
        while(iter != tokens.end()) {  //take every word and check if its NOT
            if(*iter != "NOT") {        // go through the rest of the vect,until we Hit NOT
                string find = *iter;
                boost::algorithm::to_lower(find);
                if(!readstop.stops.contains(find)) {
                    Porter2Stemmer::stem(find);
                    or_vect.push_back(readQuery(find));
                    ors.push_back(find);
                }
                iter++;
            }
            else { //if we found not
                iter++;
                while(iter != tokens.end()) { //add all nots
                    string find = *iter;
                    boost::algorithm::to_lower(find);
                    if(readstop.stops.contains(find)) {
                        Porter2Stemmer::stem(find);
                        nots.push_back(find);
                        not_vect.push_back(readQuery(find));
                    }
                    iter++;
                }
                return;
            }
        }
    }
    else { //only one word
        string find = *iter;
        boost::algorithm::to_lower(find);
        Porter2Stemmer::stem(find);
        //we're going to add that to or vector anyway
        or_vect.push_back(readQuery(find));
        return;
    }
}

//You must use this method regardless of Boolean value
/**
 * @brief queryProcessor::readQuery : this takes all of our seperate vectors and loads in the word from
 * the index value, and then we compare all of our results\n
 * this is essentially the heart of the user input section of the project
 * @param find : this is our individual word that we are going to analyze
 * @return : this may look really complex, but think of it as a pair with key string and the value is essentially
 *  an ID# a boolean set to true for AND words, and a double for the total score
 */
pair<string,vector<pair<pair<int,bool>,double>>> queryProcessor::readQuery(string find) {
    reads.open("indexInterface.txt");
    pair<string,vector<pair<pair<int,bool>,double>>> WordQue;
    if(!reads) {
        cerr << "indexInterface could not be opened!";
        exit(EXIT_FAILURE);
    }
    else {
        int totalQ,totalWord;
        string top50;
        reads >> totalQ;
        reads >> totalWord;
        reads.ignore('\n');
        getline(reads, top50,'\n');
        string buffer;
        //We use while true since writing IndexInt. creates newline at the end
        while(true) {
            getline(reads,buffer);
            if(buffer.empty())
                break;
            //tokenize and stem every word in the query
            boost::char_separator<char> sep(" ");
            boost::tokenizer< boost::char_separator<char> > tok(buffer, sep);
            boost::tokenizer<boost::char_separator<char>>::iterator iter = tok.begin();
            //if the word in the index file is our word we're looking for, save all the ID and score values
            if(*iter == find) {
                WordQue.first = find;
                iter++;
                while(iter != tok.end()) {
                    //turn them all into proper casts and store them into our add pair
                    int id = boost::lexical_cast<int>(*iter);
                    iter++;
                    double score = boost::lexical_cast<double>(*iter);
                    pair<pair<int,bool>,double> add;
                    add.first.first = id;
                    add.first.second = false;
                    add.second = score;
                    WordQue.second.push_back(add);
                    iter++;
                    }
                break;
                }
            }
        //sort all the queues into ascending order so most important comes first
        sort(WordQue.second.begin(), WordQue.second.end(), []( const std::pair<pair<int,bool>, int> &p1,
             const std::pair<pair<int,bool>, double> &p2 )
        {
            return (p1.second > p2.second);
        } );
        reads.close();
        return WordQue;
    }

}
