//#include <iostream>
//#include <ctime>
//#include <unistd.h>
//using namespace std;

//int main(int argc, char *argv[])
//{
//    cout << "Hello World!" << endl;

//    time_t start = time(NULL);
//    for(int i = 0 ; i < 3 ; i++)
//    {
//        sleep(4);
//    }
//    time_t end = time(NULL);

//    cout << " total time" << difftime(end,start);
//    return 0;
//}



#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
int main() {

    struct timeval  tv;
    gettimeofday(&tv, NULL);

    double begin_seconds =(tv.tv_sec) * 1000 ;
    double begin_milliseconds = (tv.tv_usec) / 1000 ;

    usleep(2600000);

    gettimeofday(&tv, NULL);

    double end_seconds =(tv.tv_sec) * 1000 ;
    double end_milliseconds = (tv.tv_usec) /1000 ;

//    printf("Execution time %f\n", end_seconds - begin_seconds);
//    printf("Execution time %f\n", end_milliseconds - begin_milliseconds);

    printf("%f\n", (end_milliseconds+ end_seconds) - (begin_milliseconds+ begin_seconds));

    return (0);
}

///* Program to demonstrate time taken by function fun() */
//#include <stdio.h>
//#include <time.h>

//// A function that terminates when enter key is pressed
//void fun()
//{
//    printf("fun() starts \n");
//    printf("Press enter to stop fun \n");
//    while(1)
//    {
//        if (getchar())
//            break;
//    }
//    printf("fun() ends \n");
//}

//// The main program calls fun() and measures time taken by fun()
//int main()
//{
//    // Calculate the time taken by fun()
//    clock_t t;
//    t = clock();
//    fun();
//    t = clock() - t;
//    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

//    printf("fun() took %f seconds to execute \n", time_taken);
//    return 0;
//}
