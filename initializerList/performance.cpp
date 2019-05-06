// Without Initializer List

class MyClass {
    Type variable;
public:
    MyClass(Type a) { 
      variable = a;
    }
};
Here compiler follows following steps to create an object of type MyClass
1. Type’s constructor is called first for “a”.
2. The assignment operator of “Type” is called inside body of MyClass() constructor to assign

	variable = a; 

3. And then finally destructor of “Type” is called for “a” since it goes out of scope.


//With initializer list
class MyClass {
    Type variable;
public:
    MyClass(Type a):variable(a) {   
    }
};
With the Initializer List, following steps are followed by compiler:
1. Copy constructor of “Type” class is called to initialize : variable(a). The arguments in initializer list are used to copy construct “variable” directly.
2. Destructor of “Type” is called for “a” since it goes out of scope.
