/* Wilson 
 * nxor.cpp
*/
#include "nxor.h"

NXOR::NXOR():Components(){
    this->label = "NXOR";
}

int NXOR::executeHelper(int a, int b){
    return a^b;
}

int NXOR::getOutput(){
    return Components::getOutput(true);
}
