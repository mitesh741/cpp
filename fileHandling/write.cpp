#include<fstream>
using namespace std;

int main()
{
	ofstream fout;
	fout.open("out.txt");

	char str[300] = "Time is a great teacher but unfortunately it kkills all its pupils";

//	fout << str;

fout.write(str,sizeof(str));

	fout.close();
	return 0 ;
}
