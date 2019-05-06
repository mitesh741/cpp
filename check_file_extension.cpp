#include <iostream>
#include <string>

int main()
{
	std::string fn = "filename.CON";
	if( !((fn.substr(fn.find_last_of(".") + 1) == "conf") || (fn.substr(fn.find_last_of(".") + 1) == "CONF")) ) 
	{
		std::cout << "No..." << std::endl;
	} else
	{
		std::cout << "yes..." << std::endl;
	}
}
