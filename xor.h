/* Wilson 
 * xor.h
*/
#ifndef XOR_H
#define XOR_H
#include "components.h"
#include <string>
#include <iostream>
class XOR : public Components {
public:
    XOR();
private:
    int executeHelper(int a, int b);
};

#endif // XOR_H
