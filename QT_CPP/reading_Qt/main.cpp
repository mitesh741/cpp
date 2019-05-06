#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char *argv[])
{
    string inFileName = "text.txt";

    ifstream inFile;  // fstream

    inFile.open(inFileName); // ios::in


    if(inFile.is_open())
    {
        string line;
        //        inFile >> line;
        while(!inFile.eof()) // while(inFile) This worls too due to overloading operator
        {
            getline(inFile,line);
            cout << line <<endl;
        }
        inFile.close();
    }
    else
    {
        cout << "Cant open file" << inFileName << endl;
    }
    return 0;
}
