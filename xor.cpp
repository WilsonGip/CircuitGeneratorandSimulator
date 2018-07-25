/* Wilson 
 * xor.cpp
*/
#include "xor.h"

XOR::XOR():Components(){
    this->label = "XOR";
}

int XOR::executeHelper(int a, int b){
    return a^b;
}
