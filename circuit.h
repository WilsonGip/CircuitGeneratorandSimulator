/* Wilson 
 * circuit.h
*/
#ifndef CIRCUIT_H
#define CIRCUIT_H
#include <algorithm>
#include <bits/stdc++.h>
#include <cmath>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include "and.h"
#include "components.h"
#include "dflipflop.h"
#include "nand.h"
#include "node.h"
#include "nor.h"
#include "not.h"
#include "nxor.h"
#include "or.h"
#include "xor.h"

const int INPUT_LIMIT = 200;

struct {
    bool operator()(Node *a, Node *b){
        return ((a->getName() > b->getName()) ?
            ((!a->isFlipFlop() && b->isFlipFlop()) ||
            (!a->isFlipFlop() && a->getOutputLabel().empty() && !b->getOutputLabel().empty()))
            :(b->isFlipFlop() ||
            (!a->isFlipFlop() && !b->getOutputLabel().empty()) ||
            (!a->isFlipFlop() && a->getOutputLabel().empty())));
    }
}nodeLess;

enum MENU_TYPE {MAINMENU, MOD_COMP, SIMULATE_CIRCUIT};

class Circuit {
public:
    Circuit();
    ~Circuit();
    void menu();
private:
    template<class T>
    void getInput(T &input, std::string message){
        std::cout << message;
        std::cin >> input;
        while(std::cin.fail()){
            std::cout << "\nYou did not enter a correct value!" << std::endl;
            std::cin.clear();
            std::cin.ignore(256, '\n');
            std::cout << message;
            std::cin >> input;
        }
    }
    typedef void (Circuit::*funcAdd)(std::string);
    typedef void (Circuit::*funcMenu)();
    funcAdd handleAddArr[8];
    funcMenu handleMenuArr[4];
    bool addHelper(std::string name, Components *comp);
    bool isContained(std::string name);
    bool isContained(std::string name, std::vector<Node*> *vector);
    bool isContained(std::string name, std::vector<Node*> *vector, int &index);
    bool isFlipFlopContainedInOutput();
    bool listComponentToSelect(std::string message, std::string title);
    bool listComponentToSelect(std::string message, std::string title, std::vector<Node*> *vector);
    bool listComps(std::string message, std::string title, std::vector<Node*> *vector);
    bool requestYesOrNo(std::string message);
    std::string contentHelper(std::vector<int> *vector);
    std::string getMenu(std::string title, bool includeSelect, std::string options[], int size);
    std::string getMenu(std::string title, std::string message, bool includeSelect, std::string options[], int size);
    std::string getOptions(std::string arr[], int size);
    std::string getTable(bool isTitle);
    std::string getTableString(std::string tables[]);
    std::string getTableString(std::string tables[], bool ignoreOutput);
    std::string tableHelper(std::vector<Node*> *vector, bool isOutput = false);
    std::string tableHelper(std::vector<Node*> *vector, std::string mark, bool isOutput);
    std::string titleHelper(std::vector<Node*> *vector, std::string mark);
    std::string titleHelper(std::vector<std::string> *vector);
    void addAND(std::string name);
    void addDFLIPFLOP(std::string name);
    void addDependent();
    void addNAND(std::string name);
    void addNOR(std::string name);
    void addNOT(std::string name);
    void addNXOR(std::string name);
    void addOR(std::string name);
    void addOutput();
    void addXOR(std::string name);
    void compMenu();
    void copyPrevState();
    void copyVectorToArray(std::vector<Node*> *vector, std::string options[], unsigned int size);
    void createComp();
    void delComp();
    void deleteDependents();
    void deleteHelper(std::vector<Node*> *vector);
    void deleteVectorNodes();
    void displayCircuit();
    void displayDependents();
    void editComp();
    void execute();
    void handleAdd(unsigned int type, std::string name);
    void handleComp();
    void handleCreate();
    void handleDel();
    void handleEdit();
    void handleInput(MENU_TYPE type);
    void handleInputArr();
    void handleLink();
    void handleMain();
    void handleSim();
    void hardReset();
    void hardReset(bool request);
    void hardResetHelper();
    void initAddFuncPtr();
    void initMenuFuncPtr();
    void inputSetting();
    void linkComp();
    void mainMenu();
    void menuHelper(std::string menu, int inputSize, MENU_TYPE type);
    void printInput();
    void removeDependent();
    void requestInput(int max);
    void requestInput(int min, int max, std::string message);
    void reset();
    void resetLinks();
    void restartCircuit();
    void setOutputLabel();
    void setPreviousState();
    void setupHelper();
    void setupInput();
    void simAll();
    void simSingle();
    void simulateMenu();
    void updateCirc();
    void updatePreviousState();
    /* Class Variables*/
    Node *selected;
    bool running, isComb;
    int index;
    std::string selectedName, dash;
    std::vector<Node*> *vecNode, *outputNode, *flipFlopNode;
    std::vector<int> *vecInput, *prevState;
    std::vector<std::string> *vecInputLabel;
    unsigned int userInput, limit;
};

#endif // CIRCUIT_H
