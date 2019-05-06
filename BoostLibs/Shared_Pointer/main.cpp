/*
#include<iostream>
using namespace std;
class SmartPtr
{
    int *ptr;  // Actual pointer
public:
    // Constructor: Refer http://www.geeksforgeeks.org/g-fact-93/
    // for use of explicit keyword
    explicit SmartPtr(int *p = NULL)
    {
        ptr = p;
    }
// Destructor
~SmartPtr()
{
delete(ptr);
}
// Overloading dereferencing operator
int &operator *()
{
return *ptr;
}
};
int main()
{
SmartPtr ptr(new int());
 *ptr = 20;
 cout << *ptr;
// We don't need to call delete ptr: when the object
// ptr goes out of scope, destructor for it is automatically
// called and destructor does delete ptr.
return 0;
}
*/

#include<iostream>
using namespace std;
// A generic smart pointer class
template <class T>
class SmartPtr
{
	T *ptr;  // Actual pointer
	public:
	// Constructor
	explicit SmartPtr(T *p = NULL) { ptr = p; }

	// Destructor
	~SmartPtr() 
	{
		delete(ptr); 
	}

	// Overloading dereferncing operator
	T & operator * () 
	{
		return *ptr; 
	}

	// Overloding arrow operator so that members of T can be accessed
	// like a pointer (useful if T represents a class or struct or
	// union type)
	T * operator -> () 
	{
		return ptr;
	}
};

int main()
{
	SmartPtr<int> ptr(new int());
	*ptr = 20;
	cout << *ptr;
	return 0;
}

