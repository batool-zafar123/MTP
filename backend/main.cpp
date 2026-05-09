#include <iostream>
#include "main.h"
#include <fstream>
#include <vector>
using namespace std;

int Portfolio::start = 0;
int Portfolio::end = 0;

// Base class to handle file reading and shared data (HTML + Markdown)

fstream Portfolio::openFile(string path, ios::openmode mode){
    fstream file(path, mode);
    if(!file.is_open()){
        cerr << "File open failed: " << path << endl;
    }
    return file;         
}

Portfolio :: Portfolio(){
    fstream inputFile = Portfolio::openFile("input.md", ios :: in);
    input = Portfolio::readContent(inputFile);

    // Checking the basic layout of md
    if (
        input.find("# PROFILE") == string::npos ||
        input.find("# SKILLS") == string::npos ||
        input.find("# PROJECTS") == string::npos || 
        input.find("# CONTACT") == string::npos
        ) {
                cout << "Error: Basic layout of md is corrupted...\n";
                exit(1); // Give error to the website
            }
           
        fstream htmlFile = Portfolio::openFile("templates/template.html", ios::in);
        html = Portfolio::readContent(htmlFile);

}

string Portfolio :: parsing() {
    return "";
}

// Reads entire file content into a single string

string Portfolio :: readContent(fstream &file){
    string line;
    string content;
    while(getline(file, line)){
         content += line + "\n";
    }
    return content;
}

// Profile

// Replaces profile placeholders in HTML using input.md data

string Profile :: parsing(){
    int i = 0;

    // Find next '[' and extract text inside brackets
    while((start = input.find('[', start)) != string::npos){
        end = input.find(']', start);
        if(end == string::npos) break;

        // Extract content between '[' and ']'
        details[i++] = input.substr(start + 1, end - start - 1);

        // Move forward to avoid re-reading same section
        start = end + 1;
        if(i > 3) break;
    }

    // Stop after required number of elements (4 for profile)
    if (i < 4) {
        cout << "Error: Less than 4 profile details found...\n";
        exit(1);
    }
            
    string result;

    for (i = 0; i < html.size(); ++i) {
        if (html[i] == '[') {
                    
            // Check if current position matches placeholder
            if (html.compare(i, 14, "[PROFILE_NAME]") == 0){
                result += details[0];
                i += 13; // Skip placeholder length after replacement
            } 
            else if (html.compare(i, 15, "[PROFILE_TITLE]") == 0){
                result += details[1];
                i += 14;
            } 
            else if (html.compare(i, 13, "[PROFILE_BIO]") == 0){
                result += details[2];
                i += 12;
            } 
            else if (html.compare(i, 15, "[PROFILE_QUOTE]") == 0){
                result += details[3];
                i += 14;
            } 
            else {
                result += html[i]; // concatenate the regular text
            }
        }
        else{
            result += html[i];
        }
    }
            
    return result;
}   

// Skills 

// Extracts skills and injects into HTML template

string Skills :: parsing(){
    int i = 0;
    string category[3] = {"Languages", "Tools", "Concepts"};

    start = input.find("Languages", start); 
    while((start = input.find('[', start)) != string::npos){

        end = input.find(']', start);
        if(end == string::npos) break;
            
        details[i++] = input.substr(start + 1, end - start - 1);
        start = end + 1;

        // Stop after 3 skills
        if(i >= 3) break;
    }

    // Ensuring if user entered 3 profile details in the skills section
    if (i < 3) {
        cout << "Error: Less than 3 skills found...\n";
        exit(1);
    }    

    string skills;
    if(i >= 3){
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

    for (int j = 0; j < html.size(); ++j){
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

// Projects

// Parses multiple projects and generates dynamic HTML

string Projects :: parsing(){
    total_projects = 0;
    int i = 0;
    vector<string> title;
    vector<string> description;
    vector<string> tech;
    vector<string> links;

    // Locate each project block
    while((start = input.find("[Project Title", start)) != string :: npos){
        start += 1;
        total_projects++;
            
        // Extract title, description, tech, and link sequentially
        start = input.find("Title", start);
        if (start == string::npos) break;
        start += 16;
        end = input.find(']', start);
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

        // Checking if the elements in each project section are complete
    if (title.size() != total_projects || 
        description.size() != total_projects || 
        tech.size() != total_projects || 
        links.size() != total_projects) {
        cout << "Error: Elements missing in projects section...\n";
        exit(1);
    }

    string projects = "";

    // Generate sidebar links for each project
    for (int i=0; i<size(title); i++) {
        projects += R"(<a href="#project-)" + to_string(i+1) + R"(" class="nav-sublink">)" + title[i] + R"(</a>)";
    }

    string project_details = "";
    for(int i = 0; i < total_projects; i++){

    // Generate detailed project cards
    project_details +=  R"(<div class="project-card" id="project-)" + to_string(i+1) + R"(">
                        <h3 class="project-title">)" + title[i] + R"(</h3>
                        <p class="project-desc">)" + description[i] + R"(</p>
                        <div class="project-tech">)" + tech[i] + R"(</div>
                        <a href=")" + links[i] + R"(" class="project-link" target="_blank">View Source</a>
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

// Contact

// Extracts contact links and inserts into HTML

string Contact :: parsing(){
    int i = 0;
    string links[3];
    start = 0;

    // Start extracting from Email section
    start = input.find("Email", start);
    while((start = input.find('[', start)) != string::npos){
        end = input.find(']', start);
        if(end == string::npos) break;
        links[i++] = input.substr(start + 1, end - start - 1);
        start = end + 1;
        // Extract 3 links: Email, GitHub, LinkedIn
        if(i >= 3) break;
    }

    // Checking if user gave 3 links in the contacts section
    if (i < 3) {
        cout << "Error: Less than 3 links found...\n";
        exit(1);
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

int main() {

    Portfolio *ptr[4] = {new Profile, new Skills, new Projects, new Contact};
    
    for (int i=0; i<4; i++) {
        if (i != 0) {
            ptr[i]->html = ptr[i-1]->html;
        }
        ptr[i]->html = ptr[i]->parsing();
    }
    ofstream indexFile("output/index.html");
    if (indexFile.is_open()) {
        indexFile << ptr[3]->html;
        indexFile.close();
    } else {
        cerr << "Failed to write output to file 'index.html'\n";
    }

}