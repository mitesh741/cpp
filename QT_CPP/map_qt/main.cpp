#include <iostream>
#include <map>

using namespace std;

int main(int argc, char *argv[])
{
    map<string, int> ages;

    ages["Mitesh"] = 25;
    ages["Patel"] = 24;
    ages["Kinjal"] = 26;

    cout << ages["Mitesh"] << endl;

    for(map<string,int>::iterator it= ages.begin() ; it != ages.end() ; it++)
    {
    cout << it->first << ": " << it->second << endl;
    }
    return 0;
}
