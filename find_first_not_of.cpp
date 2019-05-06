


// string::find_first_of
#include <iostream>       // std::cout
#include <string>         // std::string
#include <cstddef>        // std::size_t

int main ()
{
	std::string str ("Please, eplace /the vowels in this sentence by asterisks.");
//	str.find_first_of("aeiou");

	if( false == (str.find_first_of( "/" ) == std::string::npos) ) // TODO can it be in float ?
	{
		std::cout << " Contains slash " ;
	}
else
{
		std::cout << " Does not Contains slash " ;

}



	return 0;
}
