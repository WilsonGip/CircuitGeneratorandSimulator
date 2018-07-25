/* Wilson 
 * and.h
*/
#ifndef AND_H
#define AND_H
#include "components.h"
#include <string>
#include <iostream>
class AND : public Components {
public:
    AND();
private:
    int executeHelper(int a, int b);
};

#endif // AND_H
