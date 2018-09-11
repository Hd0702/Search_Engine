#include "reader.h"
#include "boost/format.hpp"
#include "boost/algorithm/string/join.hpp"
#include "boost/bind.hpp"
using namespace std;
reader::reader()
{
}
/**
* @brief readsTagsNames: This is the beginning prompt for reading tags /n
* it handles the tagName file and stores into a vector for parsing in readsTags()
*
**/
void reader::readsTagsNames() {
    reads.open("tagNames.txt");
    if(!reads) {
        cerr << "TagNames could not be opened" << endl;
        exit(EXIT_FAILURE);
    } //read all tag files
    else {
        while(reads.good()) {
            string name;
            getline(reads, name);
            Tags.push_back(name);
        }

    }
    reads.close();
    readsTags();
}
/**
 * @brief reader::stopWords : this function reads the stopwords text file and stores it
 */
void reader::stopWords() {
    //Here is where we get our list of stop words
    string reader;
    reads.open("stopWords.txt");
    if(!reads) {
        cout << "stopWord File could not be opened" << endl;
        exit(EXIT_FAILURE);

    }
    else {
        while(reads.good()) {
            reads >> reader;
            stops.insert(reader);
        }
    }
    reads.close();
}
/**
 * @brief readsTags :
 * Method handles the parsing and storing tags into a map
 *
 */
void reader::readsTags() {
    string reader;
    char buffer[5000];
    char * token;
    int id;
    for(unsigned int i =0; (Tags.size() == 0) ? i < 1 : i < Tags.size(); i++) {
        reads.open(Tags[i].c_str());
        if(!reads) {
            cout << Tags[i] << " could not be opened" << endl;
        }
        else {
            reads.ignore(500, '\n');
            while(reads.get(buffer, 500, '\n')) {  //read tags
                token = strtok(buffer, "|");
                id = atoi(token);
                token = strtok(NULL, "\n <>,/?;:'\"|][{}=_)(*&^%$@!`~");
                if(!tTags.contains(id) && token != NULL) { //if the id doesnt exist
                    string a = token;  //we can make this more efficent
                    Porter2Stemmer::stem(a);
                    if(!rawData.contains(a)) {
                        rData.push_back(a);
                        rawData.insert(a);
                        count++;
                   }
                    if(!unqId.contains(id)) {
                        unqId.insert(id);

                        tagCheck.push_back(id);
                    }
                    tTags.insert(id); //now it does exist
                    //this is each id in each thing
                    tTags[id].push_back(a);
                }
                else if(token != NULL && tTags.contains(id)) {
                    string a = token;  //we can make this more efficent
                    Porter2Stemmer::stem(a);
                    tTags[id].push_back(a); //adding to a tag
                }
                reads.ignore(1, '\n');
            }
        }
        reads.close();
    }

}
/**
  *@brief reader : Reads file names from questionNames, for file persistance when adding new question.psv files
  **/
void reader::readQuestionNames(int pass) {
    //maybe we could pass in boolean for new file name?
    reads.open("questionNames.txt");
    if(!reads) {
        cerr << "questionNames could not be opened" << endl;
        exit(EXIT_FAILURE);
    }
    else {
        while(reads.good()) {
            string name;
            getline(reads, name);
            Qs.push_back(name);
        }

    }
    reads.close();
    readQuestion(pass,false,"-1");
}
//add bool for displaying full contents
/**
 * @brief reader::readQuestion : this method actually parses and stores each part of each question int a structure
 * @param check : check is the id we pass if we want to display /nfor queryprocessor, 0 displays nothing
 * @param display : we turn on the display bool if we want to display the entire question
 * @param fix : fix is an extra question name from matinance mode that load our seralized values
 */
void reader::readQuestion(int check, bool display,string fix) {
    string buffer;
    char token[5];
    string add;
    int id;
    if(fix == "20") { //we will pass in new file name so size will be 0;
        readNShow(); // now titles is populated
    }
    int counter = 0;
    ifstream b ("indexInterface.txt");
    if(b && fix == "20") {
        int temp = 0;
        b >> temp;
        totalQs += temp;
        b >> temp;
        count += temp;
        b >> temp;
        totalsize += temp;
    }
    b.close();
    //load into IDS
    if(fix == "20"){
        ifstream red("IDX.txt");
        boost::archive::text_iarchive ok(red);
        ok & IDs;
    }
    for(unsigned int i =0; (Qs.size() ==0) ? i < 1 : i < Qs.size(); i ++) {
        reads.open(Qs[i]);
        if(!reads) {
            cout << Qs[i] << " could not be opened" << endl;
        }
        else {
            reads.ignore(500,'\n'); //ignore first line
            while(reads.good()) { //read until the end of the file  //maybe we should r
                getline(reads, buffer,'|');
                id = atoi(buffer.c_str()); //first id
                if(id != 0) {
                    if(!unqId.contains(id)) {
                        unqId.insert(id);
                        tagCheck.push_back(id);
                    }
                    if(check == id) {
                        cout << "\033[0;96mID# " << id << endl;
                    }
                  IDs.push_back(id);
                  totalQs++;
                  getline(reads,buffer,'|');
                  if(check == id) {
                    cout << buffer << endl;
                  }
                  getline(reads,buffer,'|');
                  if(check == id) {
                      cout << buffer << "\033[0;0m" << endl;
                      if(!display) {
                        reads.close();
                        return;
                      }
                  }
                    getline(reads,buffer, '|'); //score
                    if(check == id) {
                        cout <<"\033[0;96m" << buffer << endl;
                    }
                    getline(reads, buffer, '|');
                    if(check == 0) {
                        vector<string> words = tokens(buffer);
                        titles.push_back(words);
                    }
                    else if(check == id) {
                        cout << buffer << endl;
                    }
                    title:
                    getline(reads,buffer, '|'); //now its stopping at the pipe before
                    reads.unget();
                    reads.get(token,3);
                    if((token[1] == '\"')) { //double check for eding code sequence
                        reads.get(token,2);
                        if(token[0] == '<'){
                            reads.unget();
                        }
                        else
                            goto title;
                    }
                    else if(token[1] == '<') {
                        reads.get(token,2);
                        if(token[0] == '>')
                        {
                            reads.get(token,2);
                            if(token[0] == '!') { //end of the code
                                reads.unget();
                            }
                            else goto title;
                            reads.unget();
                        }
                        else goto title;
                        reads.unget();
                    }
                    else{
                        if(display && check == id) {
                            cout << " " << buffer << " ";
                        }
                        goto title;
                    }
                    if(display && check == id) {
                        cout << buffer  << endl;
                    }
                    reads.ignore(6);
                    check:
                    getline(reads,buffer, '!');
                    reads.unget();
                    if(check == id && display) {
                        cout << buffer << endl;
                    }
                    reads.get(token,3);
                    //test if next character is invalid
                    if(token[0] == '\0') {
                        break;
                    }
                    reads.unget();
                    char total[10];
                    reads.get(total,6); //check for ending code sequence
                    counter ++;
                    if(strcmp(total, "<>!<>") ==0)  {//weve reached the end of code
                        reads.ignore(500, '\n'); //ignore the next line
                        if(check == id && display) {
                            cout << "\033[0;0m";
                            reads.close();
                            return;
                        }
                    }
                    else {
                        goto check;
                    }
                }

            }
            reads.close();
        }
    }
    {
        ifstream test("titleFile.txt");
        write();
    }
    {//now we write ids to thing
        ofstream test("IDX.txt");
        boost::archive::text_oarchive ok(test);
        ok & IDs;
    }
    //now we should edit our file and add new file name
    if(Qs.size() == 0) {
        ofstream adding("questionNames.txt", ios::app);
        adding << fix << endl;
        adding.close();
    }
}
/**
 * @brief reader::tokens : We use boost to tokenize every word in every
 * question and then we return each individualized stem question
 * @param body : body is the original question body we are passing in that is getting stemmed
 * @return : we return a vector via std::move into a vector of stemmed words for TFIDF
 */
vector<string> reader::tokens(string  body) {
    boost::char_separator<char> sep(" !@$%^&*()_=[]{};`~'\",/?<>:\n");
    boost::tokenizer<boost::char_separator<char>> tok(body,sep);
    vector<string> vTokens;
    for(boost::tokenizer<boost::char_separator<char>>::iterator iter = tok.begin(); iter != tok.end(); iter++) {
        string a = boost::algorithm::to_lower_copy(*iter); //maybe this is why its slower
        if(!stops.contains(a)) {
            Porter2Stemmer::stem(a);
            if(!rawData.contains(a)) {  //New word!!
                count++;
                rData.push_back(a);
                rawData.insert(a);
                rawData[a] = 1;
            }
            else
                rawData[a] = rawData[a] +1;
            vTokens.push_back(a);
            totalsize++;
        }
    }
    return vTokens;
}
/**
 * @brief reader::addQ : this function reads all files in directory that user has input and reads them
 * @param find : find is the directory name from user input
 */
void reader::addQ(string find) {
    boost::filesystem::path new_p(find);  //check if it is a directory
    if(!boost::filesystem::is_directory(new_p)) {
        cerr << "file is not directory" << endl;
        exit(EXIT_FAILURE);
    }
    boost::filesystem::directory_iterator end_itr;
    //write directory to file
    ofstream op("questionNames.txt", ios::app);
    for(boost::filesystem::directory_iterator itr(new_p); itr != end_itr; itr++) {
        op << itr->path().string() << endl;
        Qs.push_back(itr->path().string());
    }
    op.close();
    //parse only the new files
    readQuestion(0,false,"20");
}
/**
 * @brief reader::readNShow : this loads our serialized titles vector into titles for fast parsing
 */
void reader::readNShow() {
    ifstream ok("titleFile.txt");
    if(!ok) {
        cerr << "FIle not reads";
    }
    else {
        boost::archive::text_iarchive id(ok);
        id & titles;
    }
}
/**
 * @brief reader::write : this writes our titles vector into a new file for fast loading in matinance mode
 */
void reader::write() {
    ofstream ok("titleFile.txt");
    boost::archive::text_oarchive os(ok);
    os & titles;
}
