#ifndef ANIMAL_H
#define ANIMAL_H
#include <iostream>
using namespace std;
class Animal
{
protected:
    string voice;
public:
    Animal();
   virtual ~Animal();
   virtual string getVoice() { return voice ; }  // This equals makes abstract class
//    virtual string getVoice() = 0; //Abstract class
};
class Dog : public Animal
{
    string sign;
public:
    Dog();
    ~Dog();
    string getVoice() {return voice +  " " +sign;}
};
class Cat : public Animal
{
    string sign;
public:
    Cat();
    ~Cat();
    string getVoice() {return voice +  " " + sign;}
};
class Cow : public Animal
{
    string sign;
public:
    Cow();
    ~Cow();
    string getVoice() {return voice +   " " + sign;}
};
#endif // ANIMAL_H
