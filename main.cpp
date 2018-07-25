/* Wilson 
 * main.cpp
*/
#include <iostream>
#include <string>
#include "circuit.h"
using namespace std;

int main() {
    Circuit *circ = new Circuit();
    circ->menu();
    delete circ;
    return 0;
}
