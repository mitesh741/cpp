#include<iostream>
#include<vector>

using namespace std;

int main()
{
	vector<int> first;

	cout << "Size of Vector before pushing elements " << first.size() << endl;

	first.push_back(10);
	first.push_back(20);
	first.push_back(30);
	first.push_back(40);

	cout << "Size of Vector after pushing elements " << first.size() << endl;

	for(int i = 0 ; i < first.size() ; i++)
		cout << first[i] << " " << endl;

	for(int i = 0 ; i < first.size() ; i++)
		first[i] += 10;


	for(int i = 0 ; i < first.size() ; i++)
		cout << first[i] << " " <<endl;

	return 0;
}
