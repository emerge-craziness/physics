#include <string>
#include <stdio.h>
#include "MyClass.hpp"
using namespace std;

int main( int argc, char* argv[] ) {

    MyClass::initializeDefaults();
 
    int error;
    if ( argc == 2 ) {
        if ( error = MyClass::getdata() ) {
            return error;
        }

        MyClass::setFilenames( argv[1] + (string)".txt", 
                               argv[1] + (string)"_header.txt",
                               argv[1] + (string)"_info.txt");
    } 
    else {
        if ( error = MyClass::getdata() ) {
            return error;
        }

        MyClass::setFilenames( "data.txt", "data_header.txt", "data_info.txt" );
    }

    std::cout << "Processing calculations... " << std::flush;
    MyClass::calculation();
    MyClass::printdata();
 
    return 0;
}
