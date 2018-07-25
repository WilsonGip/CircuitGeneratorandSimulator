/* Wilson 
 * or.h
*/
#ifndef OR_H
#define OR_H
#include "components.h"
#include <string>
#include <iostream>
class OR : public Components {
public:
    OR();
private:
    int executeHelper(int a, int b);
};

#endif // OR_H
