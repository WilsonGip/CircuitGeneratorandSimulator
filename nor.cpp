/* Wilson 
 * nor.h
*/
#include "nor.h"

NOR::NOR():Components(){
    this->label = "NOR";
}

int NOR::executeHelper(int a, int b){
    return a||b;
}

int NOR::getOutput(){
    return Components::getOutput(true);
}
