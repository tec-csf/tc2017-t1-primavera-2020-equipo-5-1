#include <iostream>
#include <cstring>
#include <iomanip>


using namespace std;

class Instruction{
    public:
    int noLine;
    string instruction;
    string cycle;
    string complexity;
    int eo;

   
    Instruction(string instruction, int noLine){
        this->instruction = instruction;
        this->noLine = noLine;
        cycle = "1";
        complexity = "0";
        eo = 0;
    }

    template<typename T> void printElement(T t, T t2, T t3, T t4, T t5,const int& width)
    {
    cout << '|' << setw(5) << t << '|' << setw(30) << t2 << '|' << setw(30) << t3 << '|' << setw(45) << t4 << '|' << setw(5) << t5 << '|'<<endl;

    
    }

    void printInstruction(){
        string eo_s= to_string(eo);
        string no_s= to_string(noLine);
        printElement(no_s, instruction.substr(0,30), cycle, complexity, eo_s, 30);
    }

    void printTitles(){
        printElement("no", "instrucción", "ciclo", "complejidad", "OE", 20);
    }
    void setComplexity(string complexity, int eo){
        this->complexity = complexity;
        this->eo = eo;
    }

};