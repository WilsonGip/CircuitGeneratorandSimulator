/* Wilson 
 * srlatch.h
*/
#ifndef SRLATCH_H
#define SRLATCH_H
#include <iostream>
#include <string>
#include <unordered_map>
#include "srlatch.h"
#include "node.h"
#include "nand.h"

class srLatch {
public:
    srLatch();
    ~srLatch();
    int getNotOutput();             // Returns this->notQ
    int getOutput();                // Returns this->Q
    std::string getName();
    void displayCircuit();          // Displays the circuit
    void execute();                 // Execute the SR Latch
    void print();
    void reset();                   // Calls softReset() and reset this->notQ
    void setInput(int s, int r);    // Set the input of the SR Latch
    void setName(std::string name); // Set the name of the SR Latch
    void setState(int state);
private:
    bool inputSet, EXEC;
    int Q, notQ, S, R;
    std::string label, name;
    std::unordered_map <std::string, Node*> *nodeMap;
    void clearMap();
    void constructCircuit();        // Construct the SR Latch
    void softReset();               // Only reset the Gates
};

#endif // SRLATCH_H
