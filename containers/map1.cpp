#include<iostream>
#include<map>

using namespace std;

int main()
{
	map <char,int> first;

	for(int i =0 ; i < 26 ; i++)
		first.insert(pair<char,int>('A'+i , 65+ i ));

	char key;

	cout << "Enter the Key" << endl;
	cin >> key;

	map<char,int> ::iterator p ;

	p = first.find(key);

	if(p != first.end())
		cout << "Key value is " << p->first << " "  << p->second << endl;
	else
		cout << "Key not found " << endl;
			return 0 ;
}
