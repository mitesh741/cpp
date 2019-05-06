//Iterators
//1. begin() – Returns an iterator pointing to the first element in the vector
//2. end() – Returns an iterator pointing to the theoretical element that follows last element in the vector
//3. rbegin() – Returns a reverse iterator pointing to the last element in the vector (reverse beginning). It moves from last to first element
//4. rend() – Returns a reverse iterator pointing to the theoretical element preceding the first element in the vector (considered as reverse end)
//Capacity
//1. size() – Returns the number of elements in the vector
//2. max_size() – Returns the maximum number of elements that the vector can hold
//3. capacity() – Returns the size of the storage space currently allocated to the vector expressed as number of elements
//4. resize(size_type g) – Resizes the container so that it contains ‘g’ elements
//5. empty() – Returns whether the container is empty


//Accessing the elements
//1. reference operator [g] – Returns a reference to the element at position ‘g’ in the vector
//2. at(g) – Returns a reference to the element at position ‘g’ in the vector
//3. front() – Returns a reference to the first element in the vector
//4. back() – Returns a reference to the last element in the vector


#include <iostream>
#include <vector>

using namespace std;

int main()
{
    vector <int> g1;
    vector <int> :: iterator i;
    vector <int> :: reverse_iterator ir;

    for (int i = 1; i <= 5; i++)
        g1.push_back(i);

    cout << "Output of begin and end\n";
    for (i = g1.begin(); i != g1.end(); ++i)
        cout << *i << " ";
cout << endl;

    cout << endl << endl;
    cout << "Output of rbegin and rend\n";
    for (ir = g1.rbegin(); ir != g1.rend(); ++ir)
        cout  << *ir << " ";

cout<< endl;
    return 0;
}


/*
#include <iostream>
#include <vector>
using namespace std;
int main()
{
    vector <int> g1;
    for (int i = 1; i <= 5; i++)
        g1.push_back(i);

    cout << "Size : " << g1.size();
    cout << "\nCapacity : " << g1.capacity();
    cout << "\nMax_Size : " << g1.max_size() << endl;
    return 0;
}
*/
/*
#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main()
{
    vector <int> g1;

    for (int i = 1; i <= 10; i++)
        g1.push_back(i * 10);

    cout << "Reference operator [g] : g1[2] = " << g1[2];
    cout << endl;
    cout << "at : g1.at(4) = " << g1.at(4);
    cout << endl;
    cout << "front() : g1.front() = " << g1.front();
    cout << endl;
    cout << "back() : g1.back() = " << g1.back();
    cout << endl;

    return 0;

}*/
