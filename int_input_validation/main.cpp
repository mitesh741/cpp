#include <iostream>
#include <stdlib.h>
using namespace std;

int main(int argc, char *argv[])
{

    string input;

    cout <<"Enter your data: " << endl;

    cin >> input ;

    bool has_only_digits = (input.find_first_not_of( "0123456789" ) == string::npos);

    if(has_only_digits)
    {

        if( atoi(input.c_str()) < 500 )
        {
            cout << "Entered data is integer and less than 500" << endl;
        }
        cout << "Entered data is only digits " <<endl;
    }
    else
    {
        cout << "Entered data contains characters. Please insert integer value" << endl;
        return 0;
    }

    cout << "User input: "<< atoi(input.c_str()) << endl;

    return 0;
}
