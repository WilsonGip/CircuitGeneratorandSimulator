/* Wilson 
 * components.h
*/
#ifndef COMPONENTS_H
#define COMPONENTS_H
#include <string>
#include <iostream>
#include <vector>
class Components {
public:
    Components();
    virtual ~Components();
    bool isFlipFlop();
    std::string getLabel();
    std::string getName();
    std::string getOutputLabel();
    virtual bool isExecuted();
    virtual int getNotOutput();
    virtual int getOutput();
    virtual void displayMessage(int input[], unsigned int size);
    virtual void execute();
    virtual void hardReset();
    virtual void setState(int state);
    virtual void pushData(int data);
    virtual void reset();
    void addDepend(std::string name);
    void removeDepend(std::string name);
    void setName(std::string name);
    void setOutputLabel(std::string label);
protected:
    bool EXEC, flipflop;
    int output;
    std::string label, name, outputLabel;
    std::vector<int> *inputArr;             // An array that stores the input
    std::vector<std::string> *dependArr;    // An array that stores the dependent's name
    /* Returns ~this->output if isNot is true,
        else this->output;
        @param isNot, check if the return needs to be NOT
    */
    int getOutput(bool isNot);
    virtual int executeHelper(int a, int b);
};

#endif // COMPONENTS_H
