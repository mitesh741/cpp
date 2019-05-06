#include <iostream>
#include <map>
using namespace std;

int main()
{
	map<string,int> ages;
	ages["Mitesh"] = 25;
	ages["Patel"] = 26;
	ages["Kinjal"] = 27;
	ages["Patel"] = 32;


	ages.insert(pair<string,int> ("Peter",23));
	ages.insert(make_pair("Pdbhdf",23));

	cout << ages["Patel"] << endl;

	for(map<string,int>::iterator it = ages.begin() ; it != ages.end() ; it++)
	{
		cout << it->first << ": " << it->second << endl; 
	}
	for(map<string,int>::iterator it = ages.begin() ; it != ages.end() ; it++)
	{
		pair<string,int> age = *it;
	
		cout << age.first << ": " << age.second << endl; 
	}
	if(ages.find("Patel") != ages.end())
	{
		cout << "Found Patel" << endl;
	}
	else
	{
		cout << "Key not found" << endl;
	}
	return 0 ;
}
