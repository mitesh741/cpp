#include <chrono>
#include <thread>
#include <iostream>
 
int main()
{
    std::chrono::milliseconds interval( 1 ) ; // 1 milli seconds
    for( int i = 0 ; i < 10 ; ++i )
    {
       std::cout << "tick!\n" << std::flush ;
       std::this_thread::sleep_for( interval ) ;
    }
}
