#include <string>
#include <stdio.h>
#include "MyClass.hpp"

int main( int argc, char* argv[] ) {

    MyClass::initializeDefaults();
 
    if ( argc == 3 ) {
        if (MyClass::getdata ( argv[2][0] - '0' ) == 1 ) {
            return 1;
        }
        MyClass::setFilenames( argv[1] + (std::string )".txt", 
                             argv[1] + ( std::string )"_header.txt",
                             argv[1] + ( std::string )"_info.txt");
    } 
    else if( argc == 2 ) {
        if ( MyClass::getdata( argv[1][0] - '0' ) == 1 )
        {
            return 1;
        }
        MyClass::setFilenames( "data.txt", "data_header.txt", "data_info.txt" );
    }
    else {
        if ( MyClass::getdata( 0 ) == 1 ) {
            return 1;
        }
        MyClass::setFilenames( "data.txt", "data_header.txt", "data_info.txt" );
    }
    std::cout << "Processing calculation... " << std::flush;
    MyClass::calculation();
    MyClass::printdata();
 
    return 0;
}
