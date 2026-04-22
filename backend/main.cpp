#include <iostream>
#include <fstream>
using namespace std;

class Portfolio{
    string temp;
    string input;

    // create and open html file 
    public:
        fstream openFile(string path){
        fstream file(path); 
        
        if(!file.is_open()){
            cerr << "Preview unavailable!" << endl;
        }
        return file;
    }
    
    // functions to read input and template

    // function to write in file

};

class Profile : public Portfolio{
    string name;
    string title;
    string bio;

    // functions to replace text  
    public:
    void nameReplacement(){
        fstream inputFile = Portfolio::openFile("input");
        string line;
        
        int start, end;
        while(getline(inputFile, line)){
            start = line.find('[');
            end = line.find(']', start);
            name = line.substr(start + 1, end - start + 1);
        }
    }
};

class Skills : public Portfolio{
    string labs;
    string tools;
    string concepts;

    // each member is a list item in html and for each list items replace text.
};

class Projects : public Portfolio{
    string description;
    string tech;
    string link;
};

// class Project1 : public Projects{

// };
                                            // vectors of project classes
// class Project2 : public Projects{

// };

class Contact: public Portfolio{
    string github;
    string linkedin;
    string email;

    // functions to make these hyperlinks in html;
};


int main(){
    // Portfolio p()
}