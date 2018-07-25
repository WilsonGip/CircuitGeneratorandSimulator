/* Wilson 
 * nxor.h
*/
#ifndef NXOR_H
#define NXOR_H
#include "components.h"
#include <string>
#include <iostream>
class NXOR : public Components {
public:
    NXOR();
    int getOutput();
private:
    int executeHelper(int a, int b);
};

#endif // NXOR_H
