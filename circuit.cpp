/* Wilson
 * circuit.cpp
*/
#include "circuit.h"

Circuit::Circuit(){
    this->userInput     = this->index = this->limit = 0;
    this->vecNode       = new std::vector<Node*>();
    this->outputNode    = new std::vector<Node*>();
    this->flipFlopNode  = new std::vector<Node*>();
    this->vecInput      = new std::vector<int>();
    this->prevState     = new std::vector<int>();
    this->vecInputLabel = new std::vector<std::string>();
    this->selected      = nullptr;
    this->selectedName  = "";
    this->running       = this->isComb = true;
    for(int i=0;i<40;i++){this->dash += "-";}
    this->initAddFuncPtr();
    this->initMenuFuncPtr();
}

Circuit::~Circuit(){
    this->restartCircuit();
    delete this->vecNode;
    delete this->outputNode;
    delete this->vecInput;
    delete this->vecInputLabel;
    delete this->flipFlopNode;
    delete this->prevState;
}

void Circuit::menu(){
    while(running){
        this->mainMenu();
    }
}

void Circuit::initAddFuncPtr(){
    this->handleAddArr[0] = &Circuit::addAND;
    this->handleAddArr[1] = &Circuit::addOR;
    this->handleAddArr[2] = &Circuit::addNOT;
    this->handleAddArr[3] = &Circuit::addNOR;
    this->handleAddArr[4] = &Circuit::addNAND;
    this->handleAddArr[5] = &Circuit::addXOR;
    this->handleAddArr[6] = &Circuit::addNXOR;
    this->handleAddArr[7] = &Circuit::addDFLIPFLOP;
}

void Circuit::initMenuFuncPtr(){
    this->handleMenuArr[0] = &Circuit::handleMain;
    this->handleMenuArr[1] = &Circuit::handleComp;
    this->handleMenuArr[2] = &Circuit::handleSim;
}

void Circuit::mainMenu(){
    std::string menu = "";
    std::string options[] = {"Display Components",
                              "Components",
                              "Input Options",
                              "Simulation",
                              "Restart Circuit",
                              "Exit Program",
                             };
    menu.append(this->getMenu("Menu", false, options, 6));
    this->menuHelper(menu, 6, MAINMENU);
}

void Circuit::compMenu(){
    std::string menu = "";
    std::string options[] = {"Display Components",
                             "Create Components",
                             "Delete Components",
                             "Modify Components",
                             "Exit"};
    menu.append(this->getMenu("Components", true, options, 5));
    this->menuHelper(menu, 5, MOD_COMP);
}

void Circuit::simulateMenu(){
    std::string menu = "";
    std::string options[] = {"Display Components",
                             "Simulate Single",
                             "Simulate All",
                             "Reset the Circuit State",
                             "Exit"};
    menu.append(this->getMenu("Simulation", true, options, 5));
    this->menuHelper(menu, 5, SIMULATE_CIRCUIT);
}

void Circuit::menuHelper(std::string menu, int inputSize, MENU_TYPE type){
    std::cout << menu;
    this->requestInput(inputSize);
    this->handleInput(type);
}

void Circuit::handleMain(){
    switch(this->userInput){
    case 1: this->displayCircuit();
        break;
    case 2: this->compMenu();
        break;
    case 3: this->inputSetting();
        break;
    case 4: this->simulateMenu();
        break;
    case 5: if(this->requestYesOrNo("Do you really want to restart the circuit? "
                "It will delete all components! y/n: ")){this->restartCircuit();}
        break;
    case 6: this->running = false; exit(0);
        break;
    }
    this->mainMenu();
}

void Circuit::handleComp(){
    switch(this->userInput){
    case 1: this->displayCircuit();
        break;
    case 2: this->createComp();
        break;
    case 3: this->delComp();
        break;
    case 4: this->editComp();
        break;
    case 5: this->mainMenu();
        break;
    }
    this->compMenu();
}

void Circuit::handleSim(){
    switch(this->userInput){
    case 1: system("CLS");this->displayCircuit();
        break;
    case 2: this->simSingle();
        break;
    case 3: this->simAll();
        break;
    case 4: this->hardReset(true);
        break;
    case 5: system("CLS");this->mainMenu();
        break;
    }
    this->simulateMenu();
}

void Circuit::hardReset(bool request){
    if(!request){
        this->hardResetHelper();
    }else if(this->requestYesOrNo("Are you sure you want to reset the state of the circuit? y/n: ")){
        this->hardResetHelper();
        std::string message = std::string((this->flipFlopNode->empty() ? std::string("There are no exisiting flip flop components in the circuit!\n"):std::string("The flip flop components all have been reset!"))+"\n");
        std::cout << message;
    }
}

void Circuit::hardResetHelper(){
    for(auto *node : (*this->flipFlopNode)){
        node->hardReset();
    }
}

void Circuit::simSingle(){
    if(!this->vecNode->empty()){
        this->updateCirc();
        std::bitset<INPUT_LIMIT> bits;
        bits = this->limit;
        if(!this->isComb){
            for(unsigned int k = 0, j = this->flipFlopNode->size() - 1; k < this->flipFlopNode->size(); k++, j--){
                this->flipFlopNode->at(k)->setState(bits[j]);
            }
            this->copyPrevState();
        }
        this->execute();
        std::cout << this->getTable(true) << this->getTable(false);
        this->reset();
        unsigned int actualLimit = static_cast<unsigned int>(pow(2.0, this->flipFlopNode->size()));
        if(++this->limit > actualLimit){
            this->hardReset(false);
            this->limit = 0;
        }
    }else{
        std::cout << "There are no components to simulate!\n";
    }
}

void Circuit::simAll(){
    if(!this->vecNode->empty()){
        this->updateCirc();
        this->hardReset(false);
        std::cout << this->getTable(true);
        std::bitset<INPUT_LIMIT> bits;
        unsigned int size = this->vecInput->size();
        for(unsigned long long i = 0; i < (this->isComb ? 1 : pow(2.0, this->flipFlopNode->size())); i++){
            for(unsigned long long j = 0; j < pow(2.0, size); j++){
                bits = j;
                for(unsigned int k = 0, l = size-1; k < size; k++,l--){
                    this->vecInput->at(k) = bits[l];
                }
                if(!this->isComb){
                    bits = i;
                    for(unsigned int k = 0, j = this->flipFlopNode->size() - 1; k < this->flipFlopNode->size(); k++, j--){
                        this->flipFlopNode->at(k)->setState(bits[j]);
                    }
                    this->copyPrevState();
                }
                this->execute();
                std::cout << this->getTable(false);
                this->reset();
            }
        }
    }else{
        std::cout << "There are no components to simulate!\n";
    }
}

std::string Circuit::titleHelper(std::vector<std::string> *vector){
    std::string message = "";
    for(auto str : (*vector)){
        message.append(std::string(str+"\t"));
    }
    return message;
}

std::string Circuit::titleHelper(std::vector<Node*> *vector, std::string mark){
    std::string message = "";
    for(auto *node : (*vector)){
        if(vector != this->outputNode || !node->isFlipFlop() || this->isComb){
            message.append(std::string(node->getOutputLabel()+mark+"\t"));
        }
    }
    return message;
}

std::string Circuit::getTable(bool isTitle){
    std::string message = "";
    if(isTitle){
        std::string table[4] = {this->titleHelper(this->vecInputLabel),
                this->tableHelper(this->outputNode),
                this->tableHelper(this->flipFlopNode),
                this->tableHelper(this->flipFlopNode, "+", false)};
        message.append(this->getTableString(table));
    }else{
        std::string table[4] = {this->contentHelper(this->vecInput),
                this->tableHelper(this->outputNode, true),
                this->contentHelper(this->prevState),
                this->tableHelper(this->flipFlopNode, true)};
        message.append(this->getTableString(table));
    }
    return message;
}

std::string Circuit::getTableString(std::string tables[]){
    return this->getTableString(tables, false);
}

std::string Circuit::getTableString(std::string tables[], bool ignoreOutput){
    std::string message = "\t";
    if(this->isComb){
        message.append(tables[0]);
        message.append(std::string("|\t"));
        message.append(tables[1]);
    }else{
        message.append(tables[2]);
        message.append(std::string("|\t"));
        message.append(tables[0]);
        message.append(std::string("|\t"));
        message.append(tables[3]);
        message.append(std::string("|\t"));
        if(!ignoreOutput){message.append(tables[1]);}
    }
    message.append(std::string("\n"+this->dash+this->dash+"\n"));
    return message;
}

std::string Circuit::tableHelper(std::vector<Node*> *vector, bool isOutput){
    return this->tableHelper(vector, "", isOutput);
}

std::string Circuit::tableHelper(std::vector<Node*> *vector, std::string mark, bool isOutput){
    std::string message = "";
    for(auto *node : (*vector)){
        if(vector != this->outputNode || !node->isFlipFlop() || this->isComb){
            message.append(std::string((isOutput ? std::to_string(node->output()) : node->getOutputLabel())+mark+"\t"));
        }
    }
    return message;
}

std::string Circuit::contentHelper(std::vector<int> *vector){
    std::string message = "";
    for(auto integer : (*vector)){
        message.append(std::string(std::to_string(integer)+"\t"));
    }
    return message;
}

bool Circuit::isFlipFlopContainedInOutput(){
    for(auto *node : (*this->outputNode)){
        if(node->isFlipFlop()){
            return true;
        }
    }
    return false;
}

void Circuit::copyPrevState(){
    this->prevState->clear();
    for(auto *node : (*this->flipFlopNode)){
        this->prevState->push_back(node->output());
    }
}

void Circuit::restartCircuit(){
    this->deleteVectorNodes();
    this->vecNode->clear();
    this->outputNode->clear();
    this->vecInput->clear();
    this->vecInputLabel->clear();
    this->flipFlopNode->clear();
    this->prevState->clear();
    this->userInput     = 0;
    this->index         = -1;
    this->selected      = nullptr;
    this->selectedName  = "";

}

void Circuit::execute(){
    for(auto *node : (*this->outputNode)){
        node->execute();
    }
    if(!this->isComb){
        for(auto *node : (*this->flipFlopNode)){
            node->execute();
        }
    }
}

void Circuit::reset(){
    for(auto *node : (*this->vecNode)){
        node->reset();
    }
}

void Circuit::inputSetting(){
    std::string message = "";
    std::string options[5] = {"Print Inputs",
                              "Link Components with Inputs",
                              "Setup Inputs",
                              "Reset ALL LINKS",
                             "Exit"};
    message.append(this->getMenu("Input Options", false, options, 5));
    std::cout << message;
    this->requestInput(5);
    this->handleInputArr();
}

void Circuit::setupInput(){
    system("CLS");
    bool reset = (this->vecInput->size() <= 0);
    if(!reset){
        std::string message = std::string("It looks like inputs already exists!\n"
                                          "Do you want to reset the inputs? (The links from the components will stay.) y/n? ");
        reset = this->requestYesOrNo(message);
    }
    if(reset){this->setupHelper();}
    system("CLS");
}

void Circuit::resetLinks(){
    system("CLS");
    std::string message = std::string("Are you sure that you want to reset all linked input components? y/n: ");
    if(requestYesOrNo(message)){
        for(auto *node : (*this->vecNode)){
            node->resetLinks();
        }
        std::cout << "All Components have reset their links.\n";
    }
}

void Circuit::setupHelper(){
    std::string tempName = "", index = "";

    this->vecInput->clear();
    this->vecInputLabel->clear();

    std::string message = std::string("Enter the number of unique inputs: ");
    this->requestInput(0, INPUT_LIMIT, message);
    unsigned int temp = this->userInput;

    this->vecInput->resize(temp);
    this->vecInputLabel->resize(temp);

    for(unsigned int i = 0; i < temp; i++){
        system("CLS");
        index = std::to_string(i+1);
        message = std::string("Enter index #" + index + " data: ");
        this->requestInput(0, 1, message);

        message = std::string("Enter the label for index #" + index + ": ");
        this->getInput(tempName, message);

        this->vecInput->at(i) = static_cast<int>(this->userInput);
        this->vecInputLabel->at(i) = tempName;
    }
}

void Circuit::linkComp(){
    system("CLS");
    if(this->listComponentToSelect("Choose a component to link an input to!", "Linking Component Input")){
        this->handleLink();
        this->linkComp();
    }
}

void Circuit::printInput(){
    if(this->vecInput->size() > 0){
        std::string dashLine = std::string(this->dash+"\n");
        std::string message = std::string(dashLine+"\tIndex\tLabel\tData\n"+dashLine);
        for(unsigned int i = 0; i < this->vecInput->size(); i++){
            message.append(std::string("\t"+std::to_string(i)+"\t"
                                       +this->vecInputLabel->at(i)
                                       +"\t"+std::to_string(this->vecInput->at(i))+"\n"+dashLine));
        }
        std::cout << message;
    }else{
        std::cout << "There are no inputs to print!\n";
    }
}

void Circuit::deleteVectorNodes(){
    for(auto *node : (*this->vecNode)){
        delete node;
    }
}

void Circuit::addAND(std::string name){
    this->addHelper(name, new AND());
}

void Circuit::addOR(std::string name){
    this->addHelper(name, new OR());
}

void Circuit::addNOT(std::string name){
    this->addHelper(name, new NOTCOMP());
}

void Circuit::addNOR(std::string name){
    this->addHelper(name, new NOR());
}

void Circuit::addNAND(std::string name){
    this->addHelper(name, new NAND());
}

void Circuit::addXOR(std::string name){
    this->addHelper(name, new XOR());
}

void Circuit::addNXOR(std::string name){
    this->addHelper(name, new NXOR());
}

void Circuit::addDFLIPFLOP(std::string name){
    bool successful = this->addHelper(name, new DFlipFlop());
    this->isComb = false;
    this->setOutputLabel();
    if(successful){
        this->flipFlopNode->push_back(this->selected);
    }
}

bool Circuit::addHelper(std::string name, Components *comp){
    system("CLS");
    if(!this->isContained(name)){
        Node *temp = new Node(comp, name);
        temp->setInputVector(this->vecInput);
        this->selected = temp;
        this->selectedName = name;
        this->vecNode->push_back(temp);
        std::sort(this->vecNode->begin(), this->vecNode->end(), nodeLess);
        std::cout << "The Component " << temp->getName() << " {"
                  << temp->getLabel() << "} has successfully been created!\n";
        return true;
    }else{
        std::cout << "The name you inputted already existed! Please try again!\n";
        delete comp;
        return false;
    }
}

bool Circuit::isContained(std::string name){
    return this->isContained(name, this->vecNode);
}

bool Circuit::isContained(std::string name, std::vector<Node*> *vector){
    int temp;
    return this->isContained(name, vector, temp);
}

bool Circuit::isContained(std::string name, std::vector<Node*> *vector, int &index){
    int i = 0;
    for(auto *node : (*vector)){
        if(node->getName() == name){
            index = i;
            return true;
        }
        i++;
    }
    index = -1;
    return false;
}

std::string Circuit::getMenu(std::string title, bool includeSelect, std::string options[], int size){
    std::string message = std::string("Select an option!");
    return this->getMenu(title, message, includeSelect, options, size);
}

std::string Circuit::getMenu(std::string title, std::string message, bool includeSelect, std::string options[], int size){
    std::string menu = std::string("\n"+title+"\n" + this->dash + "\n"+message);
    if(includeSelect){
        menu.append(std::string("\tSelected Component: "
                + this->selectedName + std::string(this->selected != nullptr ?
                std::string(" {" + this->selected->getLabel() + "}") : "")));
    }
    menu.append(std::string("\n" + this->dash + "\n"));
    menu.append(this->getOptions(options, size));
    return menu;
}

std::string Circuit::getOptions(std::string arr[], int size){
    std::string options = "";
    for(int i = 0; i < size; i++){
        options.append(std::string(std::to_string(i+1)+". "+arr[i]+"\n"));
    }
    options.append(std::string(this->dash+"\n"));
    return options;
}

void Circuit::requestInput(int max){
    std::string message = "Enter your option: ";
    this->requestInput(1, max, message);
}

void Circuit::requestInput(int min, int max, std::string message){
    int temp = 0;
    this->getInput(temp, message);
    while(temp < min || temp > max){
        std::cout << "You entered an integer out of range!" << std::endl;
        this->getInput(temp, message);
    }
    this->userInput = static_cast<unsigned int>(temp);
}

void Circuit::updateCirc(){
    this->isComb = this->flipFlopNode->empty();
}

void Circuit::handleInput(MENU_TYPE input){
    system("CLS");
    (this->*handleMenuArr[input])();
}

void Circuit::handleAdd(unsigned int type, std::string name){
    (this->*handleAddArr[type])(name);
}

void Circuit::createComp(){
    std::string menu = "";
    std::string options[9] = {"AND","OR","NOT","NOR","NAND","XOR","NXOR","D Flip Flop", "Exit"};
    menu.append(this->getMenu("Create Component", false, options, 9));
    std::cout << menu;
    this->requestInput(9);
    this->userInput--;
    if(this->userInput < 8){
        this->handleCreate();
    }
    if(this->userInput >= 8){
        system("CLS");
        this->compMenu();
    }
    this->createComp();
}

void Circuit::handleCreate(){
    std::string message = std::string("Name the component: ");
    std::string temp = "";
    this->getInput(temp, message);
    this->handleAdd(this->userInput, temp);
}

void Circuit::delComp(){
    system("CLS");
    if(this->listComponentToSelect("Select a Component to delete!", "Deleting Component")){
        this->handleDel();
    }
}

void Circuit::handleDel(){
    std::string message = "Are you sure about deleting {" + this->selectedName + "}? y/n: ";
    if(this->selected != nullptr && this->requestYesOrNo(message)){
        this->deleteHelper(this->vecNode);
        this->deleteHelper(this->outputNode);
        this->deleteHelper(this->flipFlopNode);
        this->deleteDependents();
        delete selected;
        std::cout << "Component {" << this->selectedName << "} has been successfully deleted!\n";
        this->selected = nullptr;
        this->selectedName = "";
    }else{
        std::cout << "There is nothing to delete!\n";
    }
}

void Circuit::deleteHelper(std::vector<Node*> *vector){
    if(this->isContained(this->selectedName, vector, this->index)){
        vector->at(static_cast<unsigned int>(this->index)) = nullptr;
        vector->erase(vector->begin()+this->index);
    }
}

void Circuit::deleteDependents(){
    for(auto *node : (*this->vecNode)){
        node->removeEdge(this->selected);
    }
}

bool Circuit::requestYesOrNo(std::string message){
    system("CLS");
    std::cout << message;
    std::string temp = "";
    std::cin >> temp;
    while(temp.at(0) != 'y' && temp.at(0) != 'n'){
        std::cout << "\nPlease enter \'y\' or \'n\'!" << std::endl;
        std::cin.clear();
        std::cin.ignore(256, '\n');
        std::cout << message;
        std::cin >> temp;
    }
    return (temp.at(0) == 'y');
}

void Circuit::editComp(){
    std::string menu = "";
    std::string options[4] = {"Set Component Dependents",
                              "Set Output Components",
                              "Remove Component Dependents",
                              "Exit"};
    menu.append(this->getMenu("Modifying Components", true, options, 4));
    std::cout << menu;
    this->requestInput(4);
    this->handleEdit();
}

void Circuit::handleEdit(){
    system("CLS");
    switch(this->userInput){
    case 1: this->addDependent();
        break;
    case 2: this->addOutput();
        break;
    case 3: this->removeDependent();
        break;
    case 4: this->compMenu();
        break;
    }
}

void Circuit::handleInputArr(){
    switch(this->userInput){
    case 1: system("CLS");this->printInput();
        break;
    case 2: this->linkComp();
        break;
    case 3: this->setupInput();
        break;
    case 4: this->resetLinks();
        break;
    case 5: system("CLS");this->mainMenu();
        break;
    }
    this->inputSetting();
}

void Circuit::handleLink(){
    if(this->vecInput->size() > 0 && this->vecNode->size() > 0){
        this->printInput();
        std::string message = std::string("Enter "+std::to_string(this->vecInput->size())+" to exit!\nSelect the input to link {" + this->selectedName
                  + "}! Enter the index: ");
        this->requestInput(0, static_cast<int>(this->vecInput->size()), message);
        if(this->userInput < this->vecInput->size()){
            this->selected->insertIndex(this->userInput);
        }
    }
}

void Circuit::addDependent(){
    if(this->vecNode->size() > 0){
        Node *comp = this->vecNode->at(0), *dependent = this->vecNode->at(0);
        while((comp && dependent) && comp == dependent){
            this->displayDependents();
            comp = (this->listComponentToSelect("Please select the component to add dependents to.", "Choose a Component!") ?
                        this->selected : nullptr);
            dependent = (comp ? (this->listComponentToSelect("Please select a dependent component!", "Choose a Dependent!") ?
                                     this->selected : nullptr) : nullptr);
            if((comp && dependent) && comp == dependent){
                std::cout << "You cannot add the component's dependent to itself.\n";
            }
        }
        system("CLS");
        if(comp && dependent){
            if(dependent->isFlipFlop()){
                std::string message = std::string("Is this component {" + this->selectedName + "} dependent on the Output or Not Output? \'y\' = output \'n\' = NOT output: ");
                comp->addEdge(dependent, this->requestYesOrNo(message));
            }else{
                comp->addEdge(dependent);
            }
            this->selected = comp;
            this->selectedName = this->selected->getName();
            this->addDependent();
        }
    }
    this->editComp();
}

void Circuit::removeDependent(){
    if(this->listComponentToSelect("Select a Component to have its Dependent removed!", "Removing Dependency")){
        Node *node = this->selected;
        std::vector<Node*> *vector = node->getDependentVector();
        if(this->listComponentToSelect("Please Select a dependent to be removed!", "Remove a Dependent", vector)){
            node->removeEdge(this->selected);
            std::cout << "The dependent " << this->selected->getName() << " {" << this->selected->getLabel()
                      << "} has been removed from " << node->getName()
                      << " {" << node->getLabel() << "}!\n";
        }
        delete vector;
        this->removeDependent();
    }
    this->editComp();
}

void Circuit::addOutput(){
    system("CLS");
    if(this->listComponentToSelect("Select a Component to set as an Output!", "Setting Output")){
        if(!this->isContained(this->selectedName, this->outputNode)){
            this->setOutputLabel();
            this->outputNode->push_back(this->selected);
            std::string message = std::string("The Component "+this->selectedName+" {"+this->selected->getLabel()+"} successfully been set as an output component!\n");
            std::cout << message;
        }
    }
}

void Circuit::setOutputLabel(){
    std::string message =  std::string("Enter the output label for " + this->selected->getName() + " {" + this->selected->getLabel() + "}: ");
    std::string temp = "";
    this->getInput(temp, message);
    this->selected->setOutputLabel(temp);
}

bool Circuit::listComponentToSelect(std::string message, std::string title){
    return this->listComponentToSelect(message, title, this->vecNode);
}

bool Circuit::listComponentToSelect(std::string message, std::string title, std::vector<Node*> *vector){
    if(this->listComps(message, title, vector)){
        this->selected = vector->at(this->userInput);
        this->selectedName = this->selected->getName();
        return true;
    }else{
        this->selected = nullptr;
        this->selectedName = "";
    }
    return false;
}

bool Circuit::listComps(std::string message, std::string title, std::vector<Node*> *vector){
    std::string menu = "";
    unsigned int size = vector->size();
    if(!vector->empty()){
        std::string options[size+1];
        this->copyVectorToArray(vector, options, size+1);
        options[size] = std::string("Exit");
        menu.append(this->getMenu(title, message, true, options, static_cast<int>(size+1)));
        std::cout << menu;
        this->requestInput(static_cast<int>(size+1));
        system("CLS");
        if(--this->userInput < size){
            return true;
        }
    }else{
        menu.append(std::string("There are no components to select from!\n"));
        std::cout << menu;
    }
    return false;
}

void Circuit::copyVectorToArray(std::vector<Node*> *vector,std::string options[], unsigned int size){
    if(size >= vector->size()){
        unsigned int i = 0;
        for(auto *node : (*vector)){
            options[i++] = std::string(node->getName()+" {"+node->getLabel()+"}");
        }
    }else{
        std::cout << "FAIL TO COPY VECTOR TO ARRAY! ARRAY SIZE TOO SMALL!\n";
    }
}

void Circuit::displayCircuit(){
    std::sort(this->vecNode->begin(), this->vecNode->end(), nodeLess);
    if(this->vecNode->size() <= 0){
        std::cout << "There are no components to display!\n";
        return;
    }
    for(auto *node : (*this->vecNode)){
        node->displayGate();
    }
}

void Circuit::displayDependents(){
    std::sort(this->vecNode->begin(), this->vecNode->end(), nodeLess);
    for(auto *node : (*this->vecNode)){
        node->displayDependents();
    }
}
