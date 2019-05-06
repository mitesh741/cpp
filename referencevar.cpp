#include<iostream>
#include<string> 
using namespace std;
int main()
{
	string name = "patel";

	cout << "Value of variable" << name << "Address "<<&name<< endl;

// string &ref_var; // Wrong . Must be initialized when created
//Refe var cant change association
 
	string &reference_var = name ;  // Reference variabel

	cout<<"Value of variable"<<reference_var<<"Address"<<&reference_var << endl;
	reference_var = "Mitesh";
	cout << "Value of variable" << name << "Address "<<&name<< endl;
	cout<<"Value of variable"<<reference_var<<"Address"<<&reference_var << endl;

	return 0;
}

