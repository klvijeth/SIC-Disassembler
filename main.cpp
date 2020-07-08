#include<iostream>
#include<bits/stdc++.h>
#include<string>
#include<cstring>
#include <algorithm>
#include <boost/bind.hpp>
using namespace std;

struct optab
{
    string opcode;
    int value;

    bool operator==(const optab& l, const optab& r)const
  {
     return l.opcode == r.opcode;
  }
};

struct symtab
{
    string label;
    int address;
};
ifstream fp_obj;
ifstream fp_opt;
ifstream fp_sym;
vector <optab> optable;
vector <symtab> symtable;

vector <string> tokenize(string line,char delimiter)
{
	stringstream check1(line);
    vector <string> tokensVec;
    string token;

    while(getline(check1,token,delimiter))
    {
        tokensVec.push_back(token);
    }
    return tokensVec;

}

int stringToHex(string num)
{
    int number;
    stringstream str;
    str<<num;
    str>>std::hex>>number;
    return number;
}

void headerConverter(string objLine)
{
    vector <string> header = tokenize(objLine,'^');
    if(header[0]=="H")
    {

    }
}

void tableGenerate()
{
    string line;
    fp_opt.open("optab.txt");
    while (getline(fp_opt, line))
    {

        vector <string> tokenized_line=tokenize(line,'\t');
        optab o1;
        o1.opcode=tokenized_line[0];
        o1.value=stringToHex(tokenized_line[1]);
        optable.push_back(o1);
    }

    fp_sym.open("symtab.txt");
    while (getline(fp_sym, line))
    {

        vector <string> tokenized_line=tokenize(line,'\t');
        symtab s1;
        s1.label=tokenized_line[0];
        s1.address=stringToHex(tokenized_line[1]);
        symtable.push_back(s1);
    }
}

int main()
{
//     fp_obj.open("objprog.txt");
//     string line;
//     vector<string> v;
//    // iterator<string> it;
//     while (getline(fp_obj, line))
//      {
//         cout<<line<<endl;
//         v=tokenize(line);
//         cout<<v[0];
//         for(string s : v)
//         {
//             cout<<s<<endl;
//         }
        

//      }
    tableGenerate();
    // for(int i=0;i<optable.size();i++)
    //     cout<<optable[i].opcode<<" "<<std::hex<<optable[i].value<<endl;

    // for(int i=0;i<symtable.size();i++)
    //     cout<<symtable[i].label<<" "<<std::hex<<symtable[i].address<<endl;
    vector<optab> :: iterator it = std::find(optable.begin(),optable.end(),optable.)


    


    
}