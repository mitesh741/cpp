#include <string>
#include <iostream>
#include <map>
using namespace std;
int main()
{
	std::map<std::string,int> my_map;

	my_map["x"] =  11;
	my_map["y"] = 23;

	auto int it = my_map.find("x");

	if (it != my_map.end())
		std::cout << "x: " << it->second << "\n";


	it = my_map.find("z");
	printf("it= %d my_map.end()= %d\n",it,my_map.end());
	if (it != my_map.end())
		cout << "z1: " << it->second << "\n";

	if (my_map["z"] == 42) std::cout << "Oha!\n";


	//The map doesn't contain the key. In this case, it will automatically add a key to the map with null value.

	//When we try to access non-existing element it creates it and gives random iterator.




	it = my_map.find("z");
	printf("it= %d my_map.end()= %d\n",it,my_map.end());
	if (it != my_map.end())
		cout << "z2: " << it->second << "\n";
}
