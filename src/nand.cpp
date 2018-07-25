/* Wilson 
 * nand.cpp
*/
#include "nand.h"

NAND::NAND():Components(){
    this->label = "NAND";
}

int NAND::executeHelper(int a, int b){
    return a&&b;
}

int NAND::getOutput(){
    return Components::getOutput(true);
}
