#include "hashindex.h"
using namespace std;
/**
 * @brief hashIndex::hashIndex : this is a constructor that we will NOT use to add files
 */
hashIndex::hashIndex()
{
}
/**
 * @brief hashIndex::getTF : this takes a reader/ reads all data
 * and then we use that data to compute TF scores from all of our data
 */
void hashIndex::getTF() {
    reads.stopWords();
    reads.readsTagsNames();
    reads.readQuestionNames(0);
    int counter =0;
     //key is id, value is each word in a title and how often it appears
    occurs.insert(reads.IDs[counter]); //now we have an id
    hash_map<string,int> temp;
    ofstream h;
    ofstream x;
    //x.open("testing.txt");
    h.open("TF.txt");
    for(auto & k :reads.titles ) {
        vector<string> keys;
        hash_map<string,double> temp;
        occurs.insert(reads.IDs[counter]); //set Id
        for(auto & p : k) { //this goes througgh each word
            if(!temp.contains(p)) { //if the word doesnt exist
                temp.insert(p);
                temp[p] = 1;
                keys.push_back(p);
            }
            else
                temp[p] += 1;
        } //now we need to go through and change each int value to a double value
        reads.uniques.push_back(keys);

        occurs[reads.IDs[counter]] = temp;
        counter++;
    }
    counter = 0;
    //This writes everything to a file
        for(auto & m: reads.uniques) { //each set of unique words
            h << reads.IDs[counter] << "\n";
            for(auto & n: m) { //each unique word
                //remember e values
                if(occurs[reads.IDs[counter]].contains(n))
                    h  << n<<" " << occurs[reads.IDs[counter]][n] << "\n";

            }
            counter ++;
        }
        h.close();

}
/**
 * @brief hashIndex::getIDF : now we use key values to do 2 things\n
 * 1: we compute IDF scores from each unique value in each questions\n
 * 2: we compute the top 50 words for our index file
 */
void hashIndex::getIDF() {
    hash_map <string, double> idfs;
    int counter =0;
    ofstream po;
    po.open("TFIDS.TXT");
    for(auto  k : reads.uniques) { // go through each unique word vector
        for(auto &h : k) {
             if(!idfs.contains(h)) {
                 allKey.push_back(h);
                 idfs.insert(h);
                 idfs[h] = 1;
              }
              idfs[h] += 1;
        }
    }
    for(auto & key : allKey) { //unique key values for finding TOP 50
        pair<string ,int> save;
        save.first = key;
        save.second = idfs[key];
        top50.push_back(save);
    }
    //now idfs has value for how many different documents it appears in
    for( auto & h : reads.rData)  {
        if(idfs.contains(h))
            idfs[h] = log(reads.titles.size()/(double)idfs[h]); //idf scores
    }
    for(auto & m :reads.uniques) {
        po << reads.IDs[counter] << "\n";
        for(auto & n: m) { //each key value in each unique
            if(occurs[reads.IDs[counter]].contains(n)) { //multiple each tf score with idf
                //temp already holds idf scores
                occurs[reads.IDs[counter]][n] = occurs[reads.IDs[counter]][n]*idfs[n];
                occurs[reads.IDs[counter]][n] = occurs[reads.IDs[counter]][n] * 10000000; //for scaling
                po << n << " " << occurs[reads.IDs[counter]][n] << "\n";
            }

        }
         counter++;

    }
    po.close();
    //Now we take go through every Tf value and multiply it by its corresponding idf value

}
/**
 * @brief hashIndex::join : this method takes both our IDF items and our TF items and merges them together
 * \nThis method allows for easy writing to the final file
 */
void hashIndex::join() {
    //in this method we pop AVL tree with key of word and value of pair w/ ID and Score
    int counter =0;

    //first we should do tags
    for (auto m : reads.uniques) { //This is for tags
        //error check
        if(reads.tTags.contains(reads.IDs[counter])) {
            for(auto & tg: reads.tTags[reads.IDs[counter]]) {
                pair<int, double> par;
                par.first = reads.IDs[counter];
                par.second = 20;
                fileC.insert(tg);
                fileC[tg].push_back(par); //now tags are added
                //maybe add avl for tag IDS
                tgID.insert(tg);
                tgID[tg].insert(reads.IDs[counter]);
            }
        }
        counter ++;
    }
    //now we have to do the idf scores
    counter =0;
    for(auto & m : reads.uniques) {
        for (auto & n : m) { //each individual word in each key
            fileC.insert(n);
            if(occurs[reads.IDs[counter]].contains(n)) {
                if(occurs[reads.IDs[counter]][n] >= 10) {
                    if(tgID.contains(n)) {  //reading Id's associated w/tags
                        if(!tgID[n].contains(reads.IDs[counter])) {
                            pair<int, double> par;
                            par.second = occurs[reads.IDs[counter]][n]; //this is the idf score
                            par.first = reads.IDs[counter];
                            fileC[n].push_back(par);
                        }
                    }
                    else {
                        pair<int, double> par;
                        par.second = occurs[reads.IDs[counter]][n]; //this is the idf score
                        par.first = reads.IDs[counter];
                        fileC[n].push_back(par);
                    }
                }
            }
        }
        counter++;
    }
}
/**
 * @brief hashIndex::writeFile :  Now we are actually writing the index interface file\n
 * we use our final items and format the data using TFIDF and ID# for easy searching from user perspective
 */
void hashIndex::writeFile() {

    //this actually writes the index interface file
    ofstream write;
    write.open("indexInterface.txt");
    for(auto  word: reads.rData) { //each individual word
       hash_map<int,int> save;
       if(fileC.contains(word)) {
            write << word << " ";
            for(auto & pairs : fileC[word]) { // go through each pair
                if(!save.contains(pairs.first)) {
                    write << pairs.first << " " << pairs.second << " ";
                    save.insert(pairs.first);
                }
            }
            write << endl;
       }
    }
    write.close();
    cout << "written " << endl;
}
/**
 * @brief hashIndex::evaluate : This is where we are loading the data into our Index file\n
 * This is the heart of the index file, we are combining user input and data together
 * @param qur : this is  the query that the user entered
 */
void hashIndex::evaluate(string qur) {
    queryProcessor proc(qur);
    proc.splitQuery();
    if(proc.or_vect.empty()) { //we evaluate all of our ands
        for( auto & keys: proc.and_vect) { //go through our and vector
            for(auto & item : keys.second) { //go through each ID and score
                //so item is pair<pair<int,bool>,double
                //and total is int,pair<double,bool>
                if(!total.contains(item.first.first)) { //add a new item
                    total.insert(item.first.first);
                    proc.Ids.push_back(item.first.first);
                    proc.and_Counter.insert(item.first.first);
                    proc.and_Counter[item.first.first] =0;
                    total[item.first.first].first = item.second;
                }
                else {  //if the ID has been indexed/ THIS IS WHERE AND HAPPENS
                    proc.and_Counter[item.first.first] += 1;
                    total[item.first.first].first += item.second;

                }
            }
        }
        for(auto & m : proc.Ids) {
            if(proc.and_Counter[m] == proc.and_vect.size()-1) {
                final.insert(m);
                final[m] = total[m].first;
                pair<int,double> save;
                save.first = m;
                save.second = total[m].first;
                proc.save_this.push_back(save);
            }
        }
        //Now we handel ORS
    }
        else  {
            for(auto & keys : proc.or_vect) {
                for(auto & item : keys.second) {
                    if(!total.contains(item.first.first)) { //add a new item
                        total.insert(item.first.first);
                        proc.Ids.push_back(item.first.first); //for iterating later
                        total[item.first.first].first = item.second;
                    }
                    else
                        total[item.first.first].first += item.second;
                }
           }
            for(auto & m : proc.Ids) {
                final.insert(m); //this holds id relevant
                pair<int,double> save;
                final[m] = total[m].first;
                save.first = m;
                save.second = final[m];
                proc.save_this.push_back(save);
            }
    }
    if(!proc.not_vect.empty()) { //if nots exist
        total.makeEmpty();
        proc.Ids.clear();
        for(auto & keys : proc.not_vect) {
            for(auto & item : keys.second) {
                if(!total.contains(item.first.first)) {
                    proc.Ids.push_back(item.first.first); //for iterating
                    total.insert(item.first.first);
                }
            }
        }
        for(auto & m: proc.Ids) {
            if(final.contains(m))
                 final.remove(m);
        }
    }
    //sort our results in ascending order so the most important result comes first
    sort(proc.save_this.begin(),proc.save_this.end(), []( const std::pair<int,double> &p1,
         const std::pair<int,double> &p2 )
    {
        return(p1.second > p2.second);
    } );
    int i =0;
    //push back the first 15 values so we dont have too many
    for(auto & m : proc.save_this) {
        if(final.contains(m.first)) {
            i++;
            rests.push_back(m.first);
            if(i == 15)
                break;
        }
    }
    if(rests.empty())
        cout << "Query did not provide any results \n";
    ifstream reads("indexInterface.txt");
    //load in statistics now from index
    reads >> totalQ;
    reads >> totalWord;
    reads.ignore(300, '\n');
    getline(reads, tops50,'\n');
    reads.close();
}
/**
 * @brief hashIndex::getRests : this returns the top 15 values so the user interface
 * has limited interaction with many other items in the program
 * @return : return the top 15 result ids
 */
vector<int> hashIndex::getRests() {
    return rests;
}
/**
 * @brief hashIndex::adjustStats : this adjusts the top 15 stats by refrence\n
 * this is once again so the user interface has limited interaction w/ the program
 * @param id : total # of Q's
 * @param qs : total # unique words
 * @param top : top 50 words
 */
void hashIndex::adjustStats(int & id, int & qs, string & top) {
    id = totalQ;
    qs = totalWord;
    top = tops50;
}
