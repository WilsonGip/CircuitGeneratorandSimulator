/* Wilson 
 * nand.h
*/
#ifndef NAND_H
#define NAND_H
#include "components.h"
#include <string>
#include <iostream>
class NAND : public Components {
public:
    NAND();
    int getOutput();
private:
    int executeHelper(int a, int b);
};

#endif // NAND_H
