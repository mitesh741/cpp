#include <string>
#include <iostream>
#include <map>

int main()
{
	std::map<std::string,std::string> my_map;
	std::string device;

	std::string first = "first" ; 
	std::string second = "second" ; 

	my_map.insert(std::make_pair(device,first));
	my_map.insert(std::make_pair(device,second));

	std::map<std::string, std::string>::iterator it;
	it = my_map.find(device);
	if (it == my_map.end())
	{
		std::cout << "z1: " << it->second << "\n";
	}
	else
	{
		std::cout << "here" <<std::endl ;
	}

	// Accessing a non-existing element creates it
	//	if (my_map["z"] == 42) std::cout << "Oha!\n";

	//	it = my_map.find("z");
	//	if (it != my_map.end()) std::cout << "z2: " << it->second << "\n";
}
