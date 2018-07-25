/* Wilson 
 * not.cpp
*/
#include "not.h"

NOTCOMP::NOTCOMP():Components(){
    this->label = "NOT";
}

void NOTCOMP::execute(){
    if(this->inputArr->size() > 0){
        this->EXEC = true;
        output = !inputArr->at(0);
    }else{
        std::cout << "Missing input!" << std::endl;
    }
}
