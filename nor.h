/* Wilson 
 * nor.h
*/
#ifndef NOR_H
#define NOR_H
#include "components.h"
#include <string>
#include <iostream>
class NOR : public Components {
public:
    NOR();
    int getOutput();
private:
    int executeHelper(int a, int b);
};

#endif // NOR_H
