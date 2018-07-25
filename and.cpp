/* Wilson 
 * and.cpp
*/
#include "and.h"

AND::AND():Components(){
    this->label = "AND";
}

int AND::executeHelper(int a, int b){
    return a&&b;
}
