/* Wilson 
 * srlatch.cpp
*/
#include "srlatch.h"

srLatch::srLatch(){
    this->inputSet  = false;
    this->label     = "SR Latch";
    this->name      = "";
    this->EXEC      = false;
    this->Q         = 0;
    this->notQ      = this->S = this->R = 1;
    this->nodeMap   = new std::unordered_map<std::string, Node*>();
    this->constructCircuit();
}

srLatch::~srLatch(){
    this->clearMap();
    delete this->nodeMap;
}

int srLatch::getOutput(){
    return this->Q;
}

int srLatch::getNotOutput(){
    return this->notQ;
}

void srLatch::displayCircuit(){
    for(auto it = this->nodeMap->begin(); it != this->nodeMap->end(); it++){
        it->second->displayGate();
    }
}

void srLatch::print(){
    std::cout << "S: " << this->S << " R: " << this->R << " Output: "
              << this->Q << " Not Output: " << this->notQ << std::endl;
}

void srLatch::constructCircuit(){
    this->nodeMap->clear();
    Node *NAND1 = new Node(new NAND(), "NAND1"),
            *NAND2 = new Node(new NAND(), "NAND2");

    (*this->nodeMap)["NAND1"] = NAND1;
    (*this->nodeMap)["NAND2"] = NAND2;

    NAND2->addEdge(NAND1);
}

void srLatch::setInput(int s, int r){
    if(r == 0){
        this->reset();
        if(s == 0){
            std::cout << "ILLEGAL VARIABLES! S and R cannot be 0 at the same time!" << std::endl;
            return;
        }
    }else{this->softReset();}
    this->inputSet = true;
    this->S = s;
    this->R = r;
    Node *NAND1 = (*this->nodeMap)["NAND1"],
         *NAND2  = (*this->nodeMap)["NAND2"];
    NAND1->addInput(this->S);
    NAND1->addInput(this->notQ);
    NAND2->addInput(this->R);
}

void srLatch::softReset(){
    this->inputSet = false;
    for(auto it = this->nodeMap->begin(); it != this->nodeMap->end(); it++){
        it->second->reset();
    }
}

void srLatch::reset(){
    this->softReset();
    this->Q     = 0;
    this->notQ  = this->S = this->R = 1;
}

void srLatch::setName(std::string name){
    this->name = name;
}

void srLatch::setState(int state){
    this->Q = state;
    this->notQ = !this->Q;
}

std::string srLatch::getName(){
    return this->name;
}

void srLatch::execute(){
    if(this->inputSet){
        Node *NAND1 = (*this->nodeMap)["NAND1"],
             *NAND2 = (*this->nodeMap)["NAND2"];
        NAND2->execute();
        this->Q         = NAND1->output();
        this->notQ      = NAND2->output();
    }else{
        std::cout << "Cannot execute because fail to set input!" << std::endl;
    }
}

void srLatch::clearMap(){
    for(auto it = this->nodeMap->begin(); it != this->nodeMap->end(); it++){
        delete it->second;
    }
}
