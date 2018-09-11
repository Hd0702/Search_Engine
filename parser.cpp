#include "parser.h"

parser::parser() {
    checkQs();
    getTF();
    getIDF();
}
void parser::checkQs() {
    ofstream p;
    p.open("final.txt");
    for( auto&  h : reads.rData) {
        p << h <<  " " <<reads.rawData[h] << "\n";
    } //test file for words and their TF score
    p.close();
    //we gotta calculate TF first//now we have value for each question
        //we need to compare it to the same value for each final value
}
void parser::getTF() { // go through each question
    int counter =0;
     //key is id, value is each word in a title and how often it appears
    occurs.insert(reads.IDs[counter]); //now we have an id
    dstree<string,int> temp;
    ofstream h;
    ofstream x;
    x.open("testing.txt");
    h.open("TF.txt");
    for(auto & k :reads.titles ) {
        //going through each individual question
        vector<string> keys;
        dstree<string,double> temp;
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

        for(auto & p : keys) {
            //we actually need to divide by the total amount of words
            //temp[p] = temp[p]/(double)reads.rawData[p];
            temp[p] = temp[p]/(double)reads.totalsize;
            x << p << " " << temp[p] << "\n"; //x is testing file
        }
        occurs[reads.IDs[counter]] = temp;
        counter++;
    }
    counter = 0;
    //This writes everything to a file
        for(auto & m: reads.uniques) { //each set of unique words
            h << reads.IDs[counter] << "\n";
            for(auto & n: m) { //each unique word
                //remember e values
                //cout << n << "N" << endl;
                if(occurs[reads.IDs[counter]].contains(n))
                    h  << n<<" " << occurs[reads.IDs[counter]][n] << "\n";

            }
            counter ++;
        }
        x.close();
        h.close();
}
void parser::getIDF() {
    /**
     * we need to know how many documents a certain word appears in
     * go through each seperate word, check uniques vector and increase the value if it appears once
     * now we will have a key for every word and a value for how many times it appears
     *then use the formula for each key value
     * go through each key in TF AVL and multiply it with its corresponding idf value
     *
     **/
    //STEP 1: find how many times every word appears.
    //go through unqiues right?
    dstree <string, double> idfs;
    int counter =0;
    ofstream po;
    po.open("TFIDS.TXT");
    for(auto  k : reads.uniques) { // go through each unique word vector
        for(auto &h : k) {
             if(!idfs.contains(h)) {
                 idfs.insert(h);
                 idfs[h] = 1;
              }
              idfs[h] += 1;
        }
    }
    //now idfs has value for how many different documents it appears in
    for( auto & h : reads.rData) {
        idfs[h] = log(reads.titles.size()/(double)idfs[h]); //idf scores
       // cout << h << " " << idfs[h] << "\n";
    }
    for(auto & m :reads.uniques) {
        po << reads.IDs[counter] << "\n";
        for(auto & n: m) { //each key value in each unique
            if(occurs[reads.IDs[counter]].contains(n)) { //multiple each tf score with idf
                //temp already holds idf scores
                occurs[reads.IDs[counter]][n] = occurs[reads.IDs[counter]][n]*idfs[n];
                occurs[reads.IDs[counter]][n] = occurs[reads.IDs[counter]][n] * 100000; //for scaling
                po << n << " " << occurs[reads.IDs[counter]][n] << "\n";
            }
            //cout << counter << endl;

        }
         counter++;

    }
    po.close();
    //Now we take go through every Tf value and multiply it by its corresponding idf value
}
