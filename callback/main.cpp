#include<stdio.h>


typedef void (*callback)(void);
void register_callback(callback ptr_reg_callback);


void my_callback(void)
{
    printf("inside my_callback\n");
}


void register_callback(callback ptr_reg_callback)
{
    printf("inside register_callback\n");
    (*ptr_reg_callback)();
}

int main(void)
{
    callback ptr_my_callback = my_callback;
    printf("function callback\n");

    register_callback(ptr_my_callback);
    printf("back inside main program\n");
    return 0;
}
