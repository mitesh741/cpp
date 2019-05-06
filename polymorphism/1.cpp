#include <iostream>
using namespace std;
class Bank
{  
	float getRateOfInterest(){return 0;}  
};  
class SBI : class  Bank
{  
	float getRateOfInterest(){return 8.4f;}  
} ; 
class ICICI : class  Bank
{  
	float getRateOfInterest(){return 7.3f;}  
}  ;
class AXIS : class  Bank
{  
	float getRateOfInterest(){return 9.7f;}  
}  ;
int  main(String args[])
{  
	Bank b;  
	b=new SBI();  
	cout << "SBI Rate of Interest: " << b.getRateOfInterest();  
	b=new ICICI();  
	cout << "ICICI Rate of Interest: "+b.getRateOfInterest();  
	b=new AXIS();  
	cout << "AXIS Rate of Interest: "+b.getRateOfInterest();  

	return 0 ;
}  

