#include<iostream>
#include<memory>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
using namespace std;

struct A;
struct B;

struct A 
{
	boost::shared_ptr<B> bptr;
	~A() 
	{
		cout << "A is deleted!" << endl;
	}
};
/*
   struct B 
   {
   boost::shared_ptr<A> aptr;
   ~B() 
   {
   cout << "B is deleted!"<< endl;
   }
   };
   */
struct B 
{
	boost::weak_ptr<A> aptr;
	~B()
	{
		cout << "B being is deleted!"<< endl;
	}
/*	void do_something() 
	{
		boost::shared_ptr<A> strongp = aptr.lock();
		if (strongp)
			cout << "A object is alive." << endl;
		else
			cout << "A object not alive." << endl;
	}*/
};

int main()
{
	boost::shared_ptr<A> ap(new A);
	boost::shared_ptr<B> bp(new B);
	ap->bptr = bp;
	bp->aptr = ap;
}  // Objects should be destroyed.


//http://www.informit.com/articles/article.aspx?p=2085179
