/* Wilson 
 * dflipflop.h
*/
#ifndef DFLIPFLOP_H
#define DFLIPFLOP_H
#include <iostream>
#include <string>
#include <unordered_map>
#include "srlatch.h"
#include "not.h"

class DFlipFlop : public Components {
public:
    DFlipFlop();
    ~DFlipFlop();
    bool isExecuted();
    int getNotOutput();
    int getOutput();
    void displayMessage(int input[], unsigned int size);
    void execute();
    void hardReset();
    void pushData(int data);
    void reset();
    void setState(int state);
private:
    int notQ, D;
    srLatch *srL;
    std::unordered_map<std::string, Node*> *nodeMap;
    void clearMap();
    void constructCircuit();
};

#endif // DFLIPFLOP_H
