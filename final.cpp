#include<iostream>
#include<string>
#include<bits/stdc++.h>
#include<bitset>
using namespace std;

/*
    File Pointer Definitions.
*/
ifstream fp_obj;
ifstream fp_opt;
ifstream fp_sym;
ofstream fp_sic;
int rflag=0,tempAddress=0x00;

/*
    Structure for Opcode and Symbol Tables.
*/
struct optab
{
    string opcode;
    int value;
};

struct symtab
{
    string label;
    int address;
};

struct dectab
{
    string type;
    int address;
};

/*
    Vector of structures of optab and symtab.
*/
vector <optab> optable;
vector <symtab> symtable;
vector <dectab> dectable;

/*
    Global Variables.
*/
int programLength = 0x00;
long long locctr = 0x00;
long long firstAddr = 0x00;

/*
    Function: stringToHex()
    Description: Converts string from file to integer
    Arguments: string number
    Return Type: integer
*/

int stringToHex(string num)
{
    int number;
    stringstream str;
    str<<num;
    str>>std::hex>>number;
    return number;
}



/*
    Function: objCodeSlice()
    Description: Separates object code to opcode,x and address.
    Arguments: integer object code.
    Return Type: integer vector of opcode.
*/

vector<int> objCodeSlice(int num)
{
    int opcode,x,address;
    vector<int> numberVector;
    bitset<24> b(num);

    string binary = b.to_string();

    opcode = stoi(binary.substr(0,8),0,2);
    x = stoi(binary.substr(8,1),0,2);
    address = stoi(binary.substr(9,15),0,2);

    numberVector.push_back(opcode);
    numberVector.push_back(x);
    numberVector.push_back(address);

    return numberVector;
}

/*
    Function: tokenize()
    Description: tokenizes each string line by given delimiter.
    Arguments: String input line and character delimiter(ascii).
    Return Type: vector of tokenized strings.
*/

vector <string> tokenize(string line, char delim)
{
	stringstream check1(line);
    vector <string> tokensVec;
    string token;

    while(getline(check1,token,delim))
    {
        tokensVec.push_back(token);
    }
    return tokensVec;

}

/*
    Function: readTable()
    Description: Reads OPTAB and SYMTAB into disassembler program.
    Arguments: None
    Return Type: Void
*/

void readTable()
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
    cout<<"Tables OPTAB and SYMTAB read into program."<<endl;
}

/*
    Function: headerConverter()
    Description: Converts header record into SIC code.
    Arguments: String header record.
    Return Type: void
*/

void headerConverter(vector<string> header)
{
    fp_sic<<header[2]<<"\t"<<header[1]<<"\t"<<"START\t"<<header[2]<<endl;
    locctr = stringToHex(header[2]);
    firstAddr = stringToHex(header[2]);
    programLength = stringToHex(header[3]);
    cout<<"\nHeader Record Converted and loaded into SIC Program"<<endl;
}

string asciiToString(string word)
{
    vector<string> chars;
    vector<char> cha;
    vector<int> ascii;
    std::string alphaWord;

    int length = word.length();
    int i = 0;
    while(length)
    {
        chars.push_back(word.substr(i,2));
        i+=2;
        length-=2;
    }

    for( string c: chars)
        cha.push_back((char)stringToHex(c));

    for (const auto &piece : cha) alphaWord += piece;

    return alphaWord;
}

void dectabGeneration()
{
    vector<string> vec;
    string line;
    int cn=1;
    fp_obj.open("objprog.txt");
    while (getline(fp_obj, line))
     {
         cn=1;
         vec = tokenize(line,'^');
         if(vec[0]=="C")
         {
            while(cn<vec.size())
            {
            if(vec[cn]=="W")
            {
                dectab d;
                d.type="WORD";
                d.address=stringToHex(vec[cn+1]);
                dectable.push_back(d);
            }
            else if(vec[cn]=="BX")
            {
                dectab d;
                d.type="BYTE^X";
                d.address=stringToHex(vec[cn+1]);
                dectable.push_back(d);
            }
            else
            {
                dectab d;
                d.type="BYTE^C";
                d.address=stringToHex(vec[cn+1]);
                dectable.push_back(d);
            }
            cn+=2;
            }
         }
         else if(vec[0]=="R")
         {
             while(cn<vec.size())
             {
             if(vec[cn]=="RW")
             {
                 dectab d;
                 d.type="RESW";
                 d.address=stringToHex(vec[cn+1]);
                 dectable.push_back(d);
             }
             else
             {
                 dectab d;
                d.type="RESB";
                d.address=stringToHex(vec[cn+1]);
                dectable.push_back(d);
             }
             cn+=2;
             }
         }

     }
     cout<<"Declaration Tab created."<<endl;
     fp_obj.close();

     cout<<"DECTAB:"<<endl;
     for(int i=0;i<dectable.size();i++)
        cout<<dectable[i].type<<":"<<std::hex<<dectable[i].address<<endl;
}

void textRecConverter(vector<string> line)
{
    cout<<std::hex<<locctr<<endl;
    if(line[0] == "T")
    {
        locctr = stringToHex(line[1]);
        if(rflag==1)
        {
            cout<<"i am in loop"<<endl;
            int count=locctr-tempAddress;
            fp_sic<<count<<endl;
            rflag=0;
        }
        int length = stringToHex(line[2]);
        for(int i=3;i<line.size();i++)
        {
            vector<int> objCode;
            int flag=0;
            fp_sic<<hex<<locctr<<"\t";
            for(symtab j:symtable)
            {
                if(j.address==locctr)
                {
                    fp_sic<<j.label<<"\t";
                    flag=1;
                }
            }
            if(flag==0)
                fp_sic<<"\t";

            int oc=stringToHex(line[i]);
            int flag2=0;
            for(dectab d : dectable)
            {
                if(d.address==locctr)
                {
                    if(d.type=="BYTE^C")
                    {
                        fp_sic<<"BYTE\t"<<"C'"<<asciiToString(line[i])<<"'"<<endl;
                        locctr+=asciiToString(line[i]).length();
                    }
                    else if(d.type=="BYTE^X")
                    {
                        fp_sic<<"BYTE\t"<<"X'"<<std::hex<<oc<<"'"<<endl;
                        locctr+=(line[i].length()/2);
                    }
                    else if(d.type=="WORD")
                    {
                        cout<<"its word"<<endl;
                        fp_sic<<"WORD\t"<<oc<<endl;
                        locctr+=3;
                    }
                    flag2=1;
                    break;
                }
            }
            if(flag2==0)
            {
                vector<int> opc;
                opc = objCodeSlice(oc);

                for(optab o:optable)
                {
                    if(o.value==opc[0])
                    {
                        fp_sic<<o.opcode<<"\t";
                    }
                }
                for(symtab s:symtable)
                {
                    if(s.address==opc[2])
                    {
                        fp_sic<<s.label;
                    }
                }
                if(opc[1]==1)
                {
                    fp_sic<<",X";
                }
                fp_sic<<endl;
                locctr+=3;
            }
        }
    }
    else
    {
        cout<<"its R record"<<endl;
        locctr=stringToHex(line[2]);
        for(int i=1;i<line.size();i+=2)
        {
            fp_sic<<hex<<locctr<<"\t";
            for(symtab j:symtable)
            {
                if(j.address==locctr)
                {
                    fp_sic<<j.label<<"\t";
                }
            }
            if(line[i]=="RW")
            {
                if((i+3)<=line.size())
                {
                    int count=stringToHex(line[i+3])-stringToHex(line[i+1]);
                    fp_sic<<"RESW\t"<<count/3<<endl;
                    locctr+=count;
                    cout<<locctr<<endl;
                }
                else
                {
                    rflag=1;
                    fp_sic<<"RESW\t";
                    tempAddress=stringToHex(line[i+1]);
                }
            }
            else
            {
                if((i+3)<line.size())
                {
                    int count=stringToHex(line[i+3])-stringToHex(line[i+1]);
                    fp_sic<<"RESB\t"<<count<<endl;
                    locctr+=count;
                }
                else
                {
                    rflag=1;
                    fp_sic<<"RESB\t";
                    tempAddress=stringToHex(line[i+1]);
                    cout<<"exiting"<<endl;
                }
            }
        }
    }
}

void endRecConverter(vector<string> objLine)
{
    string firstLabel;
    for(symtab s: symtable)
    {
        if(s.address == firstAddr)
            firstLabel = s.label;
    }

    fp_sic<<"\t"<<"END\t"<<firstLabel;
    cout<<"\t"<<"END\t"<<firstLabel;
    cout<<"\nEnd Record Converted and loaded into SIC Program."<<endl<<"End of Disassembly."<<endl;

}

int main()
{
    string line;

    readTable();
    dectabGeneration();

    fp_obj.open("objprog.txt");
    fp_sic.open("SIC2.txt");

    while (getline(fp_obj, line))
    {
        vector<string> objLine;
        objLine = tokenize(line,'^');

        if(objLine[0]=="H")
            headerConverter(objLine);

        else if(objLine[0]=="T" || objLine[0]=="R")
            textRecConverter(objLine);
        else if(objLine[0] == "E")
            endRecConverter(objLine);
    }

    fp_obj.close();
    fp_sic.close();
}
