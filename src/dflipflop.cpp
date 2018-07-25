/* Wilson 
 * dflipflop.cpp
*/
#include "dflipflop.h"

DFlipFlop::DFlipFlop(){
    this->label     = "DFlipFlop";
    this->flipflop  = true;
    this->D         = 0;
    this->notQ      = 1;
    this->srL       = new srLatch();
    this->nodeMap   = new std::unordered_map<std::string, Node*>();
    this->srL->setName("SR Latch");
    this->constructCircuit();
}

DFlipFlop::~DFlipFlop(){
    this->clearMap();
    delete this->srL;
    delete this->nodeMap;
}

void DFlipFlop::constructCircuit(){
    this->clearMap();
    Node *NOT1 = new Node(new NOTCOMP(), "NOT1");
    (*this->nodeMap)["NOT1"] = NOT1;
}

void DFlipFlop::clearMap(){
    for(auto it = this->nodeMap->begin(); it != this->nodeMap->end(); it++){
        delete it->second;
    }
}

void DFlipFlop::execute(){
    if(!this->EXEC){
        this->EXEC = true;
        int S = 0, R = 0;
        Node *NOT1 = (*this->nodeMap)["NOT1"];
        NOT1->execute();
        S = NOT1->output();
        R = this->D;
        this->srL->setInput(S, R);
        this->srL->execute();
    }
}

void DFlipFlop::hardReset(){
    this->srL->reset();
}

void DFlipFlop::setState(int state){
    this->srL->setState(state);
}

void DFlipFlop::pushData(int data){
    this->D = data;
    Node *NOT1 = (*this->nodeMap)["NOT1"];
    NOT1->reset();
    NOT1->addInput(this->D);
}

void DFlipFlop::displayMessage(int input[], unsigned int size){
    std::string message = std::string("[Gate: {" + this->label + "}\tName: {" + this->name + "}\tDependants: {");
    for(unsigned int i = 0; i < this->dependArr->size(); i++){
        message.append(this->dependArr->at(i) + (i < this->dependArr->size()-1 ? ", ": ""));
    }
    message.append(this->dependArr->size() == 0 ? "NONE, NONE" :
        (this->dependArr->size() == 1 ? ", NONE" : ""));
    message.append(std::string("}      \tOutput: {" + std::to_string(this->getOutput()) + "}\tNot Output: {"
        + std::to_string(this->getNotOutput())
        + std::string((this->outputLabel.empty() ? ""
        : std::string("}  \tOutput Label: {"+ this->outputLabel)))
        + "}\tInputs: {"));
    for(unsigned int i = 0; i < size; i++){
        message.append(std::string(std::to_string(input[i])+(i < size-1 ? ", " : "")));
    }
    message.append("}]\n");
    std::cout << message;
}

void DFlipFlop::reset(){
    this->EXEC = false;
    for(auto it = this->nodeMap->begin(); it != this->nodeMap->end(); it++){
        it->second->reset();
    }
}

int DFlipFlop::getOutput(){
    return this->srL->getOutput();
}

int DFlipFlop::getNotOutput(){
    return this->srL->getNotOutput();
}

bool DFlipFlop::isExecuted(){
    return this->EXEC;
}
