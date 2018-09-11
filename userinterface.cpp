#include "userinterface.h"
/**
 * @brief userInterface::userInterface : this constructor has nothing because this class is simply a wrapper
 *  for all the other classes
 */
userInterface::userInterface(){}

void userInterface::displayMenu() {
    cout << "\033[0;36mEnter 1 for maintenance mode or enter 2 for interactive mode\033[0m\n: ";
    int choice;
    cin >> choice;
    switch(choice) {
    case 1:
        matinance();
        break;
    case 2:
        interactive();
        break;
    default:
        cout << "Please enter a valid choice";
        this->displayMenu();
    }

}
/**
 * @brief userInterface::interactive : this method solely handles the interactive mode\n
 * it starts by checking if index exists, otherwise we create basic index
 */
void userInterface::interactive() {
    //interactive mode has blue text
    cout << "\033[0;34mInteractive Mode:\033[0;0m" << endl;
    ifstream ifile("indexInterface.txt");
    if (!ifile) { //this means index interface exists
        cout << "index file does not exist.\nCreating new index...";
        inter = new avlIndex;  //default create index using AVL tree
        inter->getTF();
        inter->getIDF();
        inter->join();
        inter->writeFile();
        //delete inter;
        }
    cout << "\033[0;34mIndex Exists. \nPlease enter 1 to load into an AVL tree"
            " or press 2 for HashMap implementation\033[0;0m" << endl;
    int choice = 0;
    cin >> choice;  //check which IndexInteface we want to load to test our queries
    switch(choice) {
    case 1:
        //green text for avlindex
        inter = new avlIndex;
        break;
    case 2:
        inter = new hashIndex;
        break;
    default:
        cout << "Choice not valid. Loading into AVL index";
        inter = new avlIndex;
    }
    if(choice == 1)
    cout << "\033[0;32mPlease enter Boolean search query in this form:\n"
            "AND java c++ c# NOT javascript\033[0;0m" << endl;
    //hashindex has magenta text
    else
        cout << "\033[0;35mPlease enter Boolean search query in this form:\n"
                "AND java c++ c# NOT javascript\033[0;0m" << endl;
    string look;
    getline(cin,look);
    getline(cin,look);
    inter->evaluate(look);  //use interface to evaluate our query
    vector<int> safe= inter->getRests();
    reader reads;
    reads.stopWords();
    if(!safe.empty()) {  //display first result
        //results have bright cyan text
        cout << "\033[0;96mResult 1: \033[0;0m\n";
        reads.readQuestionNames(safe.front());
        cout << "press enter key to continue" << endl;
        getline(cin,look);
    }
    //display the rest of the results
    for(unsigned int i =1; i < safe.size(); i ++) {
        cout << "\033[0;96mResult \033[0;0m" << i+1 <<": \n";
        reads.readQuestion(safe[i],false,"-1");
        cout << "\033[0;96mpress enter key to continue\033[0;0m" << endl;
        getline(cin,look);
    }
    if(!safe.empty()) {
        cout << "\033[0;96mplease enter number 1-"<< safe.size() <<
                " to show the questions full contents " <<
                "\nOr press -1 to skip this step\033[0;0m" << endl;
        cin >> choice;
        if(choice <= safe.size()) {
            cout << safe[choice-1] << endl;
            reads.readQuestion(safe[choice-1],true,"-1");
        }
    }
    while(true) {
        //stats have yellow text
        cout << "\033[0;33mWhich statistic would you like to print out?\n"
                "1. Total # of Questions indexed\n2. Total unique words indexed\n"
                "3. Top 50 most frequent words\n4. search again\n"
                "press 5 to quit\033[0;0m" << endl;
        cin >> choice;
        int ids = 0;
        int qs = 0;
        string top50;
        inter->adjustStats(ids,qs,top50); //change our current stats using index interface
        switch(choice) {
        //display proper stats
        case 1:
            cout << "\033[0;33mTotal Questions: \033[0;0m" << ids << "\n" << endl;
            break;
        case 2:
            cout << "\033[0;33mUnique Words: \033[0;0m" << qs << "\n" << endl;
            break;
        case 3:
            cout << "\033[0;33mTop 50 Words \n\033[0;0m" << top50 << "\n" << endl;
            break;
        case 4:
            interactive();
            break;
        default:
            exit(EXIT_SUCCESS);
        }
    }

}
/**
 * @brief userInterface::matinance : this method handles adding files to the index
 * Tags/Questions must be in seperate directory due to how data is parsed
 */
void userInterface::matinance() {
    cout << "Matinance Mode:" << endl;
    //ad option for either adding tag or question
    cout << "\033[92;44mPlease enter the following menu option\n1. Enter path for new question file "
            "and tag file\n"
            "2. Clear index file\033[0;0m" << endl;
    int choice;
    cin >> choice;
    switch(choice) {
    case 1: {
        //adding path with new question files
        //matinance mode has background colors
        cout << "\033[47;30mEnter path containing only tag files\n"
                "or enter -1 to skip this step\033[0;0m" << endl;
        string save;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, save);
        if(save != "-1") {
            boost::filesystem::path new_p(save);
            if(!boost::filesystem::is_directory(new_p)) {
                cerr << save << " is not directory" << endl;
                exit(EXIT_FAILURE);
            }
            boost::filesystem::directory_iterator end_itr;
            ofstream op("tagNames.txt", ios::app);
            for(boost::filesystem::directory_iterator itr(new_p); itr != end_itr; itr++) {
                op << itr->path().string() << endl;
            }
            op.close();
        }
        cout << "\033[46;0mEnter path for new QuestionFile to add to index: \033[0;0m";
        getline(cin,save);
        avlIndex red(save);
        red.getTF();
        red.getIDF();
        red.join();
        red.writeFile();
        break;
    }
    case 2:
        cout << "\033[31;34mdeleted index\n";
        boost::filesystem::remove("indexInterface.txt");
        boost::filesystem::remove("IDX.txt");
        boost::filesystem::remove("titleFile.txt");
        break;
    default:
        cout << "please select a compatible option" << endl;

    }
}
