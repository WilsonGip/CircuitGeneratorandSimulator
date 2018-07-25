/* Wilson 
 * node.h
*/
#ifndef NODE_H
#define NODE_H
#include <string>
#include <iostream>
#include <vector>
#include <typeinfo>
#include "components.h"

class Node{
public:
    Node(Components *comp, std::string name);
    ~Node();
    bool checkExecuting();
    bool isExecuted();
    bool isFlipFlop();
    int notOutput();
    int output();                       // @return the output of this Component
    std::string getLabel();
    std::string getName();
    std::string getOutputLabel();
    std::vector<Node *> *getDependentVector();
    void addEdge(Node *node);
    void addEdge(Node *node, bool isOutput);
    void addInput(int data);            // Pushes the data into the Component's input vector
    void displayGate();                 // Display the gate of this Component
    void displayDependents();
    void execute();
    void hardReset();
    void insertIndex(unsigned int index);
    void removeEdge(Node *node);
    void reset();                       // Resets the node
    void resetLinks();
    void setInputVector(std::vector<int> *vecInput);
    void setOutputLabel(std::string label);
    void setPrevState();
    void setStarting(bool start);
    void setState(int state);
    void updatePrevState();
private:
    struct nodePair {
    public:
        nodePair(Node *dependant = nullptr, bool isOutput = true){
            this->dependant = dependant;
            this->isOutput = isOutput;
            this->dependantName = std::string((this->isOutput ? "" : "~") + this->dependant->getName());
        }
        ~nodePair(){
            this->dependant = nullptr;
            this->isOutput  = true;
        }
        Node *dependant;
        bool isOutput;
        std::string dependantName;
    };
    Components *comp;                    // The Logic Gate that the node contains.
    bool containsInput, isExecuting;
    std::vector<int> *vecInput;
    std::vector<nodePair*> *dependArr;      // The vector that holds the other Nodes
    std::vector<unsigned int> *vecIndex;
    void copyFlipFlopDependantOutput();
    void executeDependantsAndStoreOutput();
    void grabDataFromLink();
};

#endif
