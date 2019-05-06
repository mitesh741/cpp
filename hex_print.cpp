#include<iostream>
#include<stdint.h>
#include<string.h>
using namespace std;

int main()
{  
	string text = "Å";
	uint8_t send_buffer[30];

	strcpy((char *)send_buffer,text.c_str());
	cout << "Text   "  << send_buffer << endl;
	return 0;
}
