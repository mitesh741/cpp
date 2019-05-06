#include <iostream>
#include <vector>
using namespace std;

int main(int argc, char *argv[])
{
    vector<double> numbers(0); // (20,1) Every elements is initialized with 1.

    cout << "size of vectors" << numbers.size() << endl; //Vectors manages array internally

    int capacity = numbers.capacity(); //size of internal array that vector is using

    cout << "capacity initially " << capacity << endl;
    cout << endl;

    for(int i = 0 ; i < 10; i++)
    {
        cout << numbers.capacity() << " " << capacity << "  i = " << i << endl;
        cout ;
        if(numbers.capacity() != capacity)
        {
            capacity = numbers.capacity();
            cout << "capacity " << capacity << endl << endl;

        }
        numbers.push_back(i);

    }


    return 0;
}


