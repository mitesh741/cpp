#include<stdio.h>

//see function_pointer program after this.

typedef void (*callback)(int); // callback is a type!
// typedef void(*)(int)  callback // Above statement can be read like this but its incorrect syntatically.
/*
 *typedef is used to alias types; in this case you're aliasing "callback" to void(*)().

Indeed the syntax does look odd, have a look at this:

typedef   void      (*callback)  ( );
            ^           ^         ^
     return type      type name  arguments
 */
void foo1(callback reg_callback);

callback call_mitesh;
callback call_patel;
void callback_event(int i)
{
    printf("inside callback_event callback function value of  i = %d\n", i);
}

void foo1(callback reg_callback)
{
    printf("inside foo1\n");

    for(int j = 0 ; j <= 100 ; j++)
    {
        if( (j % 10) == 0 )
        {
            (*reg_callback)(j);
        }
        printf("value of j in foor1 = %d\n", j);
    }
}
void mitesh()
{
    int i = 199;

    printf("mitesh called\n");
    call_mitesh(i);
}
void patel()
{
    int i = 200;
    printf("patel called \n");
    call_patel(i);

}
int main(void)
{
    callback ptr_callbakc_event = &callback_event;
    printf("in main function\n");

    call_patel = ptr_callbakc_event;
    call_mitesh = ptr_callbakc_event;

    mitesh();
    patel();
    foo1(ptr_callbakc_event);   // from manager init

    return 0;
}
