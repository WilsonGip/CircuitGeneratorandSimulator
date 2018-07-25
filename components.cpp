/* Wilson 
 * components.cpp
*/
#include "components.h"

Components::Components(){
    this->label     = this->name = this->outputLabel = "";
    this->output    = -1;
    this->EXEC      = this->flipflop = false;
    this->inputArr  = new std::vector<int>();
    this->dependArr = new std::vector<std::string>();
}

Components::~Components(){
    this->inputArr->clear();
    this->dependArr->clear();
    delete inputArr;
    delete dependArr;
}

void Components::execute(){
    if(this->inputArr->size() >= 2){
        this->EXEC  = true;
        int temp    = inputArr->at(0);
        for(unsigned int i = 1; i < this->inputArr->size(); i++){
            temp = (temp            == 2 ?
                   (inputArr->at(i) == 2 ? 0    : inputArr->at(i)):
                   (inputArr->at(i) == 2 ? temp :
                   this->executeHelper(temp,inputArr->at(i))));
        }
        this->output = temp;
    }else{
        std::cout << "Failed to execute " << this->name << " gate! Not enough inputs!" << std::endl;
    }
}

void Components::hardReset(){

}

void Components::setState(int state){

}

void Components::pushData(int data){
    this->inputArr->push_back(data);
}

int Components::executeHelper(int a, int b){
    return a ? b ? 0 : 0 : 0;
}

int Components::getOutput(){
    return this->getOutput(false);
}

int Components::getNotOutput(){
    return this->getOutput(true);
}

int Components::getOutput(bool isNot){
    return (this->EXEC ? (isNot ? !this->output : this->output) : 0);
}


std::string Components::getLabel(){
    return this->label;
}

std::string Components::getName(){
    return this->name;
}

void Components::setName(std::string name){
    this->name = name;
}

void Components::addDepend(std::string name){
    this->dependArr->push_back(name);
}

void Components::removeDepend(std::string name){
    unsigned int i = 0;
    for(auto str : (*this->dependArr)){
        if(str == name){
            this->dependArr->at(i) = "";
            this->dependArr->erase(this->dependArr->begin()+static_cast<int>(i));
        }
        i++;
    }
}

void Components::displayMessage(int input[], unsigned int size){
    std::string message = std::string("[Gate: {" + this->label + "}\t\tName: {" + this->name + "}\tDependants: {");
    for(unsigned int i = 0; i < this->dependArr->size(); i++){
        message.append(this->dependArr->at(i) + (i < this->dependArr->size()-1 ? ", ": ""));
    }
    message.append(this->dependArr->size() == 0 ? "NONE, NONE" :
              (this->dependArr->size() == 1 ? ", NONE" : ""));
    message.append(std::string((this->outputLabel.empty() ? "":
        std::string("}   \tOutput Label: {"+this->outputLabel))));
    message.append(std::string("}  \tInputs: {"));
    for(unsigned int i = 0; i < size; i++){
        message.append(std::string(std::to_string(input[i])+(i < size-1 ? ",":"")));
    }
    message.append(std::string("}]\n"));
    std::cout << message;
}

void Components::reset(){
    this->EXEC = false;
    this->output = -1;
    this->inputArr->clear();
}

bool Components::isExecuted(){
    return this->EXEC;
}

bool Components::isFlipFlop(){
    return this->flipflop;
}

void Components::setOutputLabel(std::string label){
    this->outputLabel = label;
    std::string message = std::string("The Component "+this->getName()+" {"+this->getLabel()
                                      +"} successfully set its output label to "+this->getOutputLabel()+"!\n");
    std::cout << message;
}

std::string Components::getOutputLabel(){
    return this->outputLabel;
}
