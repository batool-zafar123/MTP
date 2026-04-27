#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

class Portfolio{       
    public:
        string html; // string of frontend.html
        string input; // string of input.md
        static int start;  // start and end to locate the substrings.
        static int end;
        fstream openFile(string path, ios::openmode mode){
            fstream file(path, mode);
            if(!file.is_open()){
                cerr << "File open failed: " << path << endl;
            }
            return file;
        }

        Portfolio(){
            fstream inputFile = Portfolio::openFile("input.md", ios :: in);
            input = Portfolio :: readContent(inputFile);
           
            fstream htmlFile = Portfolio::openFile("templates/template.html", ios::in);
            html = Portfolio::readContent(htmlFile);

        }
        string readContent(fstream &file){
            string line;
            string content;
            while(getline(file, line)){
                content += line + "\n";
            }
            return content;
        }

};

class Profile : public Portfolio{
    public:
    string details[4]; // array for name, title, bio and quote

    string Replacement() {
        start = 0;

        int i = 0;
        while((start = input.find('[', start)) != string::npos){
            end = input.find(']', start);
            if(end == string::npos) break;

            details[i++] = input.substr(start + 1, end - start - 1);

            start = end + 1;
            if(i > 3) break;
        }
        
        string result;

        for (i = 0; i < html.size(); ++i) {
            if (html[i] == '[') {
                
                if (html.compare(i, 14, "[PROFILE_NAME]") == 0) {
                    result += details[0];
                    i += 13; // change the starting position
                } 
                else if (html.compare(i, 15, "[PROFILE_TITLE]") == 0) {
                    result += details[1];
                    i += 14;
                } 
                else if (html.compare(i, 13, "[PROFILE_BIO]") == 0) {
                    result += details[2];
                    i += 12;
                } 
                else if (html.compare(i, 15, "[PROFILE_QUOTE]") == 0) {
                    result += details[3];
                    i += 14;
                } 
                else {
                    result += html[i]; // concatenate the regular text
                }
            } 
            else {
                result += html[i];
            }
        }
        
        return result;
    }        
};

int Portfolio :: start = 0;
int Portfolio :: end = 0;

class Skills : public Portfolio{
    public:
    string details[3];
    int i = 0;
    string addSkills(){
        string category[3] = {"Languages", "Tools", "Concepts"};
        start = input.find("Languages", start); //finds Languages but returns the index of L
        while((start = input.find('[', start)) != string::npos){
            end = input.find(']', start);
            if(end == string::npos) break;
            
            details[i++] = input.substr(start + 1, end - start - 1);
            start = end + 1;
            if(i >= 3) break;
        }
        string skills;
        if(i >= 3)
        {
        skills = R"(
                <li class="skill-item">
                <span class="skill-label">)" + category[0] + R"(</span>
                <span class="skill-content">)" + details[0] + R"(</span>
                </li>
                )";
        
        skills += R"(
                <li class="skill-item">
                <span class="skill-label">)" + category[1] + R"(</span>
                <span class="skill-content">)" + details[1] + R"(</span>
                </li>
                )";
        
        skills += R"(
                <li class="skill-item">
                <span class="skill-label">)" + category[2] + R"(</span>
                <span class="skill-content">)" + details[2] + R"(</span>
                </li>
                )";
            }

        string result;

        for (int j = 0; j < html.size(); ++j) {
        if (html[j] == '[' && html.compare(j, 16, "[SKILLS_CONTENT]") == 0) {
            result += skills; // Add the generated HTML
            j += 15;            // Skip the tag "[SKILLS_CONTENT]"
        } 
        else {
            result += html[j];  // Just copy regular characters
        }
        }

        return result;
    }

    // each member is a list item in html and for each list items replace text.
};

class Projects : public Portfolio{
    public:
    int total_projects;

    string addProjects(){
        total_projects = 0;
        int i = 0;
        vector<string> title;
        vector<string> description;
        vector<string> tech;
        vector<string> linkText;
        vector<string> links;
        while((start = input.find("[Project Title", start)) != string :: npos){
            start += 1;
            total_projects++;
            
            start = input.find("Title", start);
            if (start == string::npos) break;
            start += 16;
            int end = input.find(']', start);
            if (end == string::npos) break;
            title.push_back(input.substr(start + 1, end - start - 1));

            start = input.find("Description", start);
            if (start == string::npos) break;
            start += 13;
            end = input.find(']', start);
            if (end == string::npos) break;
            description.push_back(input.substr(start + 1, end - start - 1));

            start = input.find("Tech", start);
            if (start == string::npos) break;
            start += 6;
            end = input.find(']', start);
            if (end == string::npos) break;
            tech.push_back(input.substr(start + 1, end - start - 1));

            start = input.find('(', start);
            if (start == string::npos) break;
            end = input.find(')', start);
            if (end == string::npos) break;
            links.push_back(input.substr(start + 1, end - start - 1));
                
        }

        string projects = "";

        for (int i=0; i<size(title); i++) {
            projects += R"(<a href="#project-)" + to_string(i+1) + R"(" class="nav-sublink">)" + title[i] + R"(</a>
        )";
        }

        string project_details = "";
        for(int i = 0; i < total_projects; i++){

            project_details +=  R"(<div class="project-card" id="project-)" + to_string(i+1) + R"(">
            <h3 class="project-title">)" + title[i] + R"(</h3>
            <p class="project-desc">)" + description[i] + R"(</p>
            <div class="project-tech">)" + tech[i] + R"(</div>
            <a href=)" + links[i] + R"( class="project-link" target="_blank">View Source</a>
            </div>
          )";

        }

        string result;

        for (int j = 0; j < html.size(); ++j) {
            if (html[j] == '[' && html.compare(j, 23, "[SIDEBAR_PROJECTS_LIST]") == 0) {
                result += projects;
                j += 22;            
            } 
            else if (html[j] == '[' && html.compare(j, 18, "[PROJECTS_CONTENT]") == 0) {
                result += project_details;  
                j += 17;            
            } 
            else {
                result += html[j];   
            }
        }

        return result;

    }
};

class Contact: public Portfolio{
    string email;
    string github;
    string linkedin;

    public:

    string contactDetails(){
        int i = 0;
        string links[3];
        start = 0;
        start = input.find("Email", start);
        while((start = input.find('[', start)) != string::npos){
            end = input.find(']', start);
            if(end == string::npos) break;

            links[i++] = input.substr(start + 1, end - start - 1);

            start = end + 1;
            if(i >= 3) break;
        }

        string contacts;

        contacts = R"(<a href=")" + links[0] + R"(" class="contact-link" target="_blank">
            )" + R"(<span class="contact-label">Email</span>
            )" + R"(<span class="contact-val">)" + links[0] + R"(</span>
          </a>)";
        
        contacts += R"(<a href=")" + links[1] + R"(" class="contact-link" target="_blank">
            )" + R"(<span class="contact-label">GitHub</span>
            )" + R"(<span class="contact-val">)" + links[1] + R"(</span>
          </a>)";
        
        contacts += R"(<a href=")" + links[2] + R"(" class="contact-link" target="_blank">
            )" + R"(<span class="contact-label">LinkedIn</span>
            )" + R"(<span class="contact-val">)" + links[2] + R"(</span>
          </a>)";

        string result;

        for (int j = 0; j < html.size(); ++j) {
            if (html[j] == '[' && html.compare(j, 17, "[CONTACT_CONTENT]") == 0) {
                result += contacts; // Add the generated HTML
                j += 16;            // Skip the tag "[CONTACT_CONTENT]"
            } 
            else {
                result += html[j];  // Just copy regular characters
            }
        }

        return result;
    }

};


int main() {
    Profile p;
    cout << p.details[0] << endl;
    p.html = p.Replacement();
    Skills s;
    s.html = p.html;
    s.html = s.addSkills();
    Projects pj;
    pj.html = s.html;
    pj.html = pj.addProjects();
    Contact c;
    c.html = pj.html;
    c.html = c.contactDetails();
    ofstream indexFile("output/index.html");
    indexFile << c.html;
    return 0;
}