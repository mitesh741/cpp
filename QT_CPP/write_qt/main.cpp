#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char *argv[])
{
    //    ofstream outFile;


    fstream outFile;
    string outputFileName = "text.txt";

    //    outFile.open(outputFileName);

    outFile.open(outputFileName,ios::out);

    if(outFile.is_open())
    {
        outFile << "Hello" <<endl;
        outFile << 123<< endl ;
        outFile.close();
    }
    else
    {
        cout <<"could not create file" <<outputFileName << endl;

    }

    return 0;
}
