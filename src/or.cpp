/* Wilson 
 * or.cpp
*/
#include "or.h"

OR::OR():Components(){
    this->label = "OR";
}

int OR::executeHelper(int a, int b){
    return a || b;
}
