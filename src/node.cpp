/* Wilson 
 * node.cpp
*/
#include "node.h"
Node::Node(Components *comp, std::string name){
    comp->setName(name);
    this->comp          = comp;
    this->containsInput = this->isExecuting = false;
    this->dependArr     = new std::vector<nodePair*>();
    this->vecIndex      = new std::vector<unsigned int>();
    this->vecInput      = nullptr;
}

Node::~Node(){
    for(auto *node: (*this->dependArr)){delete node;}
    this->dependArr->clear();
    this->vecIndex->clear();
    delete this->dependArr;
    delete this->comp;
    delete this->vecIndex;
    this->vecInput = nullptr;
}

void Node::execute(){
    this->isExecuting = true;
    this->grabDataFromLink();
    if(this->containsInput){
        this->executeDependantsAndStoreOutput();
        this->comp->execute();
    }else{
        std::cout << "ERROR: The gate " << this->comp->getName() << " is missing input(s)!" << std::endl;
    }
    this->isExecuting = false;
}

void Node::hardReset(){
    if(this->isFlipFlop()){
        comp->hardReset();
    }
}

void Node::setState(int state){
    if(this->isFlipFlop()){
        this->comp->setState(state);
    }
}

void Node::executeDependantsAndStoreOutput(){
    this->copyFlipFlopDependantOutput();
    int dependantsOutput = 0;
    for(auto *node : (*this->dependArr)){
        if(!node->dependant->isExecuted() && !node->dependant->checkExecuting() && !node->dependant->isFlipFlop()){
            node->dependant->execute();
        }
        if(!node->dependant->isFlipFlop()){
            dependantsOutput = node->dependant->output();
            this->comp->pushData(dependantsOutput);
        }
    }
}

void Node::copyFlipFlopDependantOutput(){
    int dependantsOutput = 0;
    for(auto *node : (*this->dependArr)){
        if(node->dependant->isFlipFlop()){
            dependantsOutput = (node->isOutput ? node->dependant->output() : node->dependant->notOutput());
            this->comp->pushData(dependantsOutput);
        }
    }
}

void Node::addInput(int data){
    this->containsInput = true;
    this->comp->pushData(data);
}

void Node::addEdge(Node *node){
    this->addEdge(node, true);
}

void Node::addEdge(Node *node, bool isOutput){
    nodePair *tempPair = new nodePair(node, isOutput);
    std::string tempName = std::string((isOutput ? "" : "~") + node->getName());
    this->containsInput = true;
    this->dependArr->push_back(tempPair);
    this->comp->addDepend(tempName);
    std::string message = std::string("The component "+this->getName()+" {"+this->getLabel()+"} has successfully added"
                                        " the dependancy "+tempName+" {"+node->getLabel()+"}!\n");
    std::cout << message;
}

void Node::removeEdge(Node *node){
    int i = 0;
    for(auto *nodeChild : (*this->dependArr)){
        if(nodeChild->dependant == node){
            this->dependArr->at(static_cast<unsigned int>(i)) = nullptr;
            this->dependArr->erase(this->dependArr->begin()+i);
            break;
        }
        i++;
    }
    this->comp->removeDepend(node->getName());
}

std::vector<Node*>* Node::getDependentVector(){
    std::vector<Node*> *vector = new std::vector<Node*>();
    for(auto *nodePair : (*this->dependArr)){
        vector->push_back(nodePair->dependant);
    }
    return vector;
}

int Node::output(){
    return this->comp->getOutput();
}

int Node::notOutput(){
    return (this->isFlipFlop() ? this->comp->getNotOutput() : this->comp->getOutput());
}

std::string Node::getLabel(){
    return this->comp->getLabel();
}

std::string Node::getName(){
    return this->comp->getName();
}

void Node::displayGate(){
    int input[this->vecIndex->size()];
    if(!this->vecIndex->empty() && this->vecInput != nullptr){
        int index = 0;
        for(auto uint : (*this->vecIndex)){
            input[index++] = this->vecInput->at(uint);
        }
    }
    this->comp->displayMessage(input, this->vecIndex->size());
}

void Node::displayDependents(){
    std::string message = std::string("[Gate: {" + this->getLabel()
                                      + "}\tName: {" + this->getName()
                                      + "}\t Dependants: {"
                                      + (this->dependArr->empty() ? "NONE" : ""));
    unsigned int i = 0, size = this->dependArr->size();
    for(auto *node : (*this->dependArr)){
        message.append(std::string(node->dependantName+(i++ < size-1 ? ", " : "")));
        i++;
    }
    message.append(std::string("}]\n"));
    std::cout << message;
}

void Node::reset(){
    this->comp->reset();
    this->isExecuting = false;
}

void Node::setOutputLabel(std::string label){
    this->comp->setOutputLabel(label);
}

void Node::setInputVector(std::vector<int> *vecInput){
    this->vecInput = vecInput;
}

void Node::insertIndex(unsigned int index){
    bool exist = false;
    for(auto integer : (*this->vecIndex)){
        if(integer == index){
            exist = true;
            break;
        }
    }
    if(!exist){
        this->vecIndex->push_back(index);
        std::cout << "The Component {" << this->getName() << "} successfully linked index "
                  << std::to_string(index) << "!\n";
    }else{
        std::cout << "The index you are trying to insert already exist!\n";
    }
}

void Node::resetLinks(){
    this->vecIndex->clear();
}

bool Node::isFlipFlop(){
    return comp->isFlipFlop();
}

bool Node::isExecuted(){
    return this->comp->isExecuted();
}

bool Node::checkExecuting(){
    return this->isExecuting;
}

std::string Node::getOutputLabel(){
    return this->comp->getOutputLabel();
}

void Node::grabDataFromLink(){
    if(this->vecIndex->size() > 0 && this->vecInput != nullptr){
        for(auto integer : (*this->vecIndex)){
            this->addInput(this->vecInput->at(integer));
        }
    }
}
