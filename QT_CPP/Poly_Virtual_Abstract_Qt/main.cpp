#include <iostream>
#include "animal.h"

using namespace std;
void voiceOfAnimal(Animal *);
//void voiceOfAnimal(Animal &);

int main(int argc, char *argv[])
{
//	Animal animal;
	Dog dog;
//	Cat cat;
//	Cow cow;
//	cout << animal.getVoice() << endl;
//	cout << cat.getVoice() << endl;
//	cout << cow.getVoice() << endl;
	//cout << dog.getVoice() << endl;

        voiceOfAnimal(&dog);
//    Animal *p = &dog;
  //  cout << p->getVoice() << endl;

	//    Animal *dog = new Animal;
	// cout << dog->getVoice() << endl;

	// delete dog;
	return 0;
}
void voiceOfAnimal(Animal *p)
{
	cout << p->getVoice() << endl;
}
/*
void voiceOfAnimal(Animal &p)
{
	cout << p.getVoice() << endl;
}*/
