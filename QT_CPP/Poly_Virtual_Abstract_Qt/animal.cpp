#include "animal.h"

Animal::Animal()
{
    voice = "default value";
    cout << "I am from the base class constructor " << endl;

}
Animal ::~Animal()
{
    cout << "I am from the base class Destructor" << endl;
}
Dog::Dog()
{
    sign  = "D";
    voice  = "hau hau hau";
    cout << "I am from the Dog class constructor " << endl;

}
Dog::~Dog()
{
    cout << "I am from the Dog class Destructor" << endl;


}
Cat::Cat()
{
    sign  = "C";

    voice  = "myau";
    cout << "I am from the Cat class constructor " << endl;


}
Cat::~Cat()
{
    cout << "I am from the Cat class Destructor" << endl;


}
Cow::Cow()
{
    sign  = "CO";

    voice  = "moooh";
    cout << "I am from the Cow class constructor " << endl;

}
Cow::~Cow()
{
    cout << "I am from the Cow class destructor" << endl;


}
