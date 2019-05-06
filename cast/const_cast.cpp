// const_cast casts away the constness of object or pointer
#include <iostream>
using namespace std;

class student
{
	private:
		int roll;
	public:
		// constructor
		student(int r):roll(r)
	{
	}

		// A const function that changes roll with the help of const_cast
		void fun() const
		{
			this->roll = 5;
		}

		int getRoll()  
		{
			return roll;
		}
};

int main(void)
{
	student s(3);
	cout << "Old roll number: " << s.getRoll() << endl;

	s.fun();

	cout << "New roll number: " << s.getRoll() << endl;

	return 0;
}
/* Inside const member function fun(), ‘this’ is treated by the compiler as ‘const student* const this’, i.e. ‘this’ is a constant pointer to a constant object, thus compiler doesn’t allow to change the data members through ‘this’ pointer. const_cast changes the type of ‘this’ pointer to ‘student* const this’. */
