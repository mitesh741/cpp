#include <iostream>
#include <vector>
using namespace std;


int main(int argc, char *argv[])
{
    vector<string> strings;             //template class

    strings.push_back("one");
    strings.push_back("two");
    strings.push_back("three");

    //    cout << strings[1] << endl;

    //    cout << strings.size() << endl;

    for(int i = 0 ; i < strings.size() ; i++)
    {
        cout << strings[i] << endl;

    }
//    vector<string>::iterator it = strings.begin();
//    it++;
//    cout << *it << endl;

    for(vector<string>::iterator it = strings.begin() ; it != strings.end() ; it++)
    {
        cout << *it << endl;

    }
    vector<string>::iterator it = strings.begin() ;
    it += 2;
    cout << *it << endl;
    return 0;
}
