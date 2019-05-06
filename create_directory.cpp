#include <stdio.h>

#define DIRNAME "/home/mitesh/Desktop/mitesh"
int main(void)
{
   int stat;
   stat = mkdir(DIRNAME);
   if (!stat)
      printf("Directory created\n");
   else
   {
      printf("Unable to create directory\n");
      exit(1);
   }
 
   return 0;
}
