#include <iostream>
using namespace std;
 
int main(void)
{
    int a1 = 40;
    const int* b1 = &a1;
    char* c1 = const_cast <char *> (b1); // compiler error
    *c1 = 'A';
    return 0;
}
/*t’safer in the sense that the casting won’t happen if the type of cast is not same as original object.*/
