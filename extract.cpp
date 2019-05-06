// string::find_last_of
#include <iostream>
#include <string>
using namespace std;

void SplitFilename (const string& str)
{
  size_t found;
  cout << "Splitting: " << str << endl;
  found=str.find_last_of("/");
  cout << " folder: " << str.substr(0,found) << endl;
  cout << " file: " << str.substr(found+1) << endl;
}

int main ()
{
  string str1 ("/home/mitesh/Desktop/Mic_sample_app");

  SplitFilename (str1);

  return 0;
}
