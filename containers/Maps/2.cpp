#include <string>
#include <iostream>
#include <map>
using namespace std;
int main()
{

	std::map<std::string, int> map;

             map=
    {
        std::make_pair("two",1 ),
        std::make_pair("one",2 )
    };




	std::map<std::string,int>::const_iterator it = map.find("two");

	if(it == map.end())
	{
		std::cout << "not found" << endl;

	}
	else
	{
		std::cout << " found" << endl;
		std::cout << it->second << endl;
	}
}

