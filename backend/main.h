#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

// BASE CLASS 
class Portfolio{       
public:
    string html;
    string input;

    static int start;
    static int end;

    Portfolio();

    virtual string parsing();

    fstream openFile(string path, ios::openmode mode);
    string readContent(fstream &file);
};

// PROFILE
class Profile : public Portfolio{
private:
    string details[4];

public:
    string parsing();
};

// SKILLS
class Skills : public Portfolio{
private:
    string details[3];

public:
    string parsing();
};

// PROJECTS
class Projects : public Portfolio{
private:
    int total_projects;

public:
    string parsing();
};

// CONTACT
class Contact : public Portfolio{
private:
    string email;
    string github;
    string linkedin;

public:
    string parsing();
};

#endif