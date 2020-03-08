
#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
#include <set> 
#include <stack>
#include <map>
#include <sstream>
#include <tuple>
#include "instruction.cpp"


using namespace std;


//clase instrucción


vector<string> split(string strToSplit, char delimeter)
{
    stringstream ss(strToSplit);
    string complete;
    vector<string> splittedStrings;
    while (getline(ss, complete, delimeter))
    {
       splittedStrings.push_back(complete);
    }
    return splittedStrings;
}

int identifyInstruction(string instruction){
    string identifiersInst[] = { "cout", "printf", "return", "if", "for", "while", "main", "else"};

    for(int i=0; i<7; i++){
        if(instruction.find(identifiersInst[i]) != string::npos){
            return i;
        }
    }
    return -1;
}

string findTypeComparison(string comparison, int operation){
     if(operation == 1){
         if(comparison.find("<=") != string::npos){
            return comparison.substr(comparison.find("=")+1);
         }
        if(comparison.find("<") != string::npos){
            return comparison.substr(comparison.find("<")+1);
        }
        if(comparison.find(">") != string::npos){
             return comparison.substr(0, comparison.find(">"));
         }
    }else if(operation == -1){
        if(comparison.find(">=") != string::npos){
            return comparison.substr(comparison.find("=")+1);
         }
        if(comparison.find(">") != string::npos){
            return comparison.substr(comparison.find(">")+1);
        }
        if(comparison.find("<") != string::npos){
             return comparison.substr(0, comparison.find("<"));
         }
    }
    return "not found";
}

string findIncrement(string increment){
    if(increment.find("+=") != string::npos || increment.find("-=") != string::npos){
        if(increment.find(")") != string::npos){
            return increment.substr(increment.find("=")+1, increment.find(")")-increment.find("=")-1);
        }else{
            return increment.substr(increment.find("=")+1, increment.find(";")-increment.find("=")-1);

        }
    }
    else if(increment.find("++") != string::npos || increment.find("--") != string::npos){
        return "";
    }else if(increment.find("+") != string::npos){
        if(increment.find(")") != string::npos){
            return increment.substr(increment.find("+")+1, increment.find(")")-increment.find("+")-1);
        }else{
            return increment.substr(increment.find("+")+1, increment.find(";")-increment.find("+")-1);
        }
    }else if(increment.find("-") != string::npos){
        if(increment.find(")") != string::npos){
            return increment.substr(increment.find("-")+1, increment.find(")")-increment.find("-")-1);
        }else{
            return increment.substr(increment.find("")+1, increment.find(";")-increment.find("-")-1);
        }
    }else{
        return "";

    }
}

string cicleTimes(string instruction){
    vector<string> partsOfFor= split(instruction, ';');

    string comparison = partsOfFor.at(1);
    string initialized = partsOfFor.at(0);
    string increment = partsOfFor.at(2);
    string temp;

    if(increment.find("++") != string::npos){
        return findTypeComparison(comparison, 1)+"-"+initialized.substr(initialized.find("=")+1);
    }
    if(increment.find("--") != string::npos){
        return initialized.substr(initialized.find("=")+1)+"-"+findTypeComparison(comparison, -1);
    }

    if(increment.find("+") != string::npos){
       return findTypeComparison(comparison, 1)+"-"+initialized.substr(initialized.find("=")+1)+"*(1/"+findIncrement(increment)+")";

    }

    if(increment.find("-") != string::npos){
        return initialized.substr(initialized.find("=")+1)+"-"+findTypeComparison(comparison, -1)+"*(1/"+findIncrement(increment)+")";
    }

    if(increment.find("*=") != string::npos){
        return " log"+increment.substr(increment.find("=")+1, increment.find(")")-increment.find("=")-1)+"("+findTypeComparison(comparison, 1)+"-"+initialized.substr(initialized.find("=")+1)+")";
    }

    if(increment.find("/=") != string::npos){
        return " log"+increment.substr(increment.find("=")+1, increment.find(")")-increment.find("=")-1)+"("+initialized.substr(initialized.find("=")+1)+"-"+findTypeComparison(comparison, -1)+")";
    }

    if(increment.find("*") != string::npos){
        return " log"+increment.substr(increment.find("*")+1, increment.find(")")-increment.find("*")-1)+"("+findTypeComparison(comparison, 1)+"-"+initialized.substr(initialized.find("=")+1)+")";
    }

     if(increment.find("/") != string::npos){
        return " log"+increment.substr(increment.find("/")+1, increment.find(")")-increment.find("/")-1)+"("+initialized.substr(initialized.find("=")+1)+"-"+findTypeComparison(comparison, -1)+")";
    }

    return "error";
}


int countElemental(string instruction){
    int num =0;
    char tmp[] = {'+', '*', '=', '-', '/', '%', '[', '<', '>'};
    string tmpdouble[] = {"++", "--", ">=", "!=", "==", "<=", "&&", "||"};
    set<char> operators(tmp, tmp+ (sizeof(tmp) / sizeof(tmp[0])));
    set<string> doubleOp(tmpdouble, tmpdouble+ (sizeof(tmpdouble) / sizeof(tmpdouble[0])));

    for(int i=0; i<instruction.length(); i++){
        string temps= "";
        temps.append(string(1,instruction[i])); 
        temps.append(string(1,instruction[i+1])); 
        if(doubleOp.count(temps) !=0){
            i++;
            num++;
        }
        else if(operators.count(instruction[i])!=0){
            num++;
        }
    }
    return num;
}


string analyzeFor(string instruction){
    vector<string> partsOfFor= split(instruction, ';');

    string comparison = partsOfFor.at(1);
    string initialized = partsOfFor.at(0);
    string increment = partsOfFor.at(2);

    int a = countElemental(initialized);
    int b = countElemental(comparison);
    int c = countElemental(increment);

    

    string cycle_temp=cicleTimes(instruction);
    return to_string(a)+"+"+to_string(b)+"*("+cycle_temp+")+"+to_string(c)+"*("+cycle_temp+"+1)";
    
}





class Analyzer{
    public:
    vector<Instruction> totalComplexity;
    string x = "";
    ifstream* inFile;
    int counterLines = 0;
    stack<string> ciclos;
    map<string , string> assignedVariables;
    map<string, int> searching;


    void readFile(int argc, char* argv[])
    {
        if(argc == 1)
    {
        cout << "no se puede leer porque no ingreso el nombre del archivo";

    }else if(strcmp(argv[1], "-i")== 0)
    {
       

        inFile = new ifstream(argv[2]);
        if(!*inFile)
        {
            cerr<< "no se puede abrir";

        }else
        {

             while(getline(*inFile, x))
             {
                
                Instruction* temp = new Instruction(x.c_str(), counterLines);
                totalComplexity.push_back(*temp);
                counterLines++;
                
            }
            inFile->close();
        }
    }
    }

    void addAsigned(string instruction)
   {
    vector<string> instruct= split(instruction, ',');
    if(instruction.find("int") != string::npos || instruction.find("float") != string::npos || instruction.find("double") != string::npos)
    {
    for(int i=0; i<instruct.size(); i++){
        string current= instruct.at(i);
    if(current.find("=") != string::npos)
    {
        string key ="'";
        if(current.find("int") != string::npos || current.find("float") != string::npos || current.find("double") != string::npos)
        {
            key = current.substr(current.find(" "), current.find("=")-3);
            
            
        }else{
            key = current.substr(0, current.find("="));
        }

        string element = current.substr(current.find("=")+1);
        assignedVariables.insert(pair<string, string> (key, element));
    }
    }
    }
   }

   string checkWhile(string instruction){
    for(auto const& [key, val] : searching){
         if(instruction.find(key) != string::npos){
             return key;
         }
    }
     return "not here";
   
}

   void analyzeWhile(string instruction, int line){
       int elemental = countElemental(instruction.substr(instruction.find("("), instruction.find("(")-1));
       string symbols[] = {"<","<=", ">",">=","==","!="};
       string delimeter ="";
        for(int i= 0; i<6; i++){
            if(instruction.find(symbols[i])!= string::npos){
                delimeter= symbols[i];
            }
        }
        
        string var1= instruction.substr(instruction.find("(")+1, instruction.find(delimeter)-instruction.find("(")-1);
        string var2= instruction.substr(instruction.find(delimeter)+1,instruction.find(")")-instruction.find(delimeter)-1);
        searching.insert(pair<string, int> (var1, line));
        searching.insert(pair<string, int> (var2, line));
        
      for(int i= line+1; i<totalComplexity.size(); i++){
          string current = totalComplexity.at(i).instruction;
        if(current != "}"){
            string temporaryWhile = checkWhile(current);
                    if(temporaryWhile != "not here"){
                        addWhile(current, temporaryWhile);
                    }
        }
      }
      totalComplexity.at(line).complexity = " "+to_string(elemental)+totalComplexity.at(line).complexity;
      ciclos.push(totalComplexity.at(line).complexity);
       totalComplexity.at(line).complexity =  totalComplexity.at(line).complexity+"+1";

    }

    void addWhile(string instruction, string variable){
        string incrementing = variable;
        int whileIndex = searching[variable];
        string initializing;
        searching.erase(variable);
         for(auto const& [key, val] : searching){
         if(whileIndex == val){
             initializing= key;
         }
        }

        searching.erase(initializing);
        if(findIncrement(instruction).empty()){
            totalComplexity.at(whileIndex).complexity = "*("+variable+"-"+initializing+")";

        }else{
            totalComplexity.at(whileIndex).complexity = "*("+variable+"-"+initializing+")"+"/"+findIncrement(instruction);

        }
        

    }
    int analyzeIF(string instruction){
        string str = instruction; // instruccion a analizar (en este caso IF)

        int posParentesis = str.find('(');
        int posParentesis2 = str.find(')');
  
        string str2 = str.substr(posParentesis+1, (posParentesis2-1)-posParentesis);
        return countElemental(str2);
    }


   void analyzeComplexity(){
    for (int i=0; i<counterLines; i++){

            string current = totalComplexity.at(i).instruction;
            if(identifyInstruction(current)< 3){

            }
            else if(identifyInstruction(current) == 3)
            {
                totalComplexity.at(i).setComplexity(to_string(analyzeIF(current)), analyzeIF(current));
                ciclos.push(" ");

            } else if(identifyInstruction(current) == 4)
            {
                totalComplexity.at(i).complexity = analyzeFor(current);
                ciclos.push(cicleTimes(current));
            }else if(identifyInstruction(current) == 5)
            {
                analyzeWhile(current, i);
            }else{
                if(current.find("{") != string::npos){
                    ciclos.push(" ");
                }
                int contElemental = countElemental(current);               
                totalComplexity.at(i).setComplexity(to_string(contElemental), contElemental);
            }

             if(!ciclos.empty()){
                stack<string> temp;
                 while(!ciclos.empty()){
                    temp.push(ciclos.top());
                    totalComplexity.at(i).cycle += ciclos.top();
                     ciclos.pop();
                 }
                 while(!temp.empty()){
                     ciclos.push(temp.top());
                     temp.pop();
                 }
             }

            if(current.find("}") != string::npos){
                
                ciclos.pop();
            }
        }
       
   }

  

    void printComplexity(){
         totalComplexity.at(0).printTitles();
    for(int i=0; i<totalComplexity.size(); i++){
        (totalComplexity.at(i)).printInstruction();
    }  
    }
   


};
int main(int argc, char* argv[])
{
   Analyzer a;
   a.readFile(argc, argv);
   a.analyzeComplexity();
   a.printComplexity();


}
