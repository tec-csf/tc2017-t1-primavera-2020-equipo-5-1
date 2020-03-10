
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

/**
 * is composed of a series of methods and functions used for the analysis of each instruction in code.cp
 * 
 * @tparam T datatype used to store variables in a polynomials
 * @tparam I datatype used to store coeficient and power of monomial
 */

template <class T, class I>
class Analyzer{
    public:
    vector<Instruction> totalComplexity;
    T x = "";
    ifstream* inFile;
    I counterLines = 0;
    stack<T> ciclos;
    T ciclePen;
    map<T ,T> assignedVariables;
    map<T, I> searching;



/**
 * Divides a string according to a prticular delimiter
 *
 * @tparam[out] vector of class T 
 * @tparam[in] string to split
 * @tparam[in] char as a delimeter
 */

template<class C>
vector<T> split(T strToSplit, C delimeter)
{
    stringstream ss(strToSplit);
    T complete;
    vector<T> splittedStrings;
    while (getline(ss, complete, delimeter))
    {
       splittedStrings.push_back(complete);
    }
    return splittedStrings;
}

/**
 * Identify instruction according to reserved words: cout, printf, if, for, while, main, else 
 *
 * @param[out] index of the instruction to be analyzed
 * @param[in] instruction as a string, where reserved words will be searched for
 */

int identifyInstruction(string instruction){
    string identifiersInst[] = { "cout", "printf", "return", "if", "for", "while", "main", "else"};

    for(int i=0; i<7; i++){
        if(instruction.find(identifiersInst[i]) != string::npos){
            return i;
        }
    }
    return -1;
}

/**
 * Finds the side of comparison to look for, according to the increment or decrement of the function
 *
 * @param[out] a string with the upper limit or lower limit of increment or decrement of a function
 * @param[in] an int indicating if the instruction is incrementing [1] or decrementing [-1]
 * @param[in] comparison string to analyze
 */

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

/**
 * Find the factor of incrementation in a for loop or while loop.
 *
 * @param[out] mean the mean of `data`, or `NaN` if `data` is empty
 * @param[out] stdDev the unbiased (sample) standard deviation, or `NaN`
 *     if `data` contains fewer than 2 elements
 * @param[in] data the data to analyze
 */

string findIncrement(string increment){
    if(increment.find("+=") != string::npos || increment.find("-=") != string::npos || increment.find("*=") != string::npos || increment.find("/=") != string::npos){
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
    }else if(increment.find("*") != string::npos){
         if(increment.find(")") != string::npos){
            return increment.substr(increment.find("*")+1, increment.find(")")-increment.find("*")-1);
        }else{
            return increment.substr(increment.find("*")+1, increment.find(";")-increment.find("*")-1);
        }
    }else if(increment.find("/") != string::npos){
         if(increment.find(")") != string::npos){
            return increment.substr(increment.find("/")+1, increment.find(")")-increment.find("/")-1);
        }else{
            return increment.substr(increment.find("/")+1, increment.find(";")-increment.find("/")-1);
        }
    }else{
        return "";
    }
}

/**
 * Calculates the amount of times a cilce will be run 
 *
 * @param[out] string with amount of times cicle will be run 
 * @param[in] cicle in the form of an instruction
 */

T cicleTimes(T instruction){
    vector<T> partsOfFor= split(instruction, ';');
    T comparison = partsOfFor.at(1);
    T initialized = partsOfFor.at(0);
    T increment = partsOfFor.at(2);

    if(increment.find("++") != string::npos){
        return (findTypeComparison(comparison, 1)+"-"+initialized.substr(initialized.find("=")+1));
    }
    if(increment.find("--") != string::npos){
        return (initialized.substr(initialized.find("=")+1)+"-"+findTypeComparison(comparison, -1));
    }

    if(increment.find("+") != string::npos){
        return findTypeComparison(comparison, 1)+"-"+initialized.substr(initialized.find("=")+1)+"* "+"1/"+findIncrement(increment);
        

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

/**
 * Counts the amount of elemental operations on a single code line
 *
 * @param[out] number of elemental operations
 * @param[in] instruction to be anylized 
 */

I countElemental(T instruction){
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

/**
 * Analyze the complexity of a for loop
 *
 * @param[out] complexity of for loop in the form of a string
 * @param[in] line of code where for loop is found
 */

T analyzeFor(T instruction){
    vector<string> partsOfFor= split(instruction, ';');

    T comparison = partsOfFor.at(1);
    T initialized = partsOfFor.at(0);
    T increment = partsOfFor.at(2);

    int a = countElemental(initialized);
    int b = countElemental(comparison);
    int c = countElemental(increment);

    
    string cycle_times= cicleTimes(instruction);
    
    return to_string(a)+"+"+to_string(b)+"*("+cicleTimes(instruction)+")+"+to_string(c)+"*("+cicleTimes(instruction)+"+1 )";
    }

/**
 * Reads the code file and saves it in a vector 
 *
 * @param[in] line of code where for loop is found
 */

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

/**
 * Saves assigned variables in a vector   
 *
 * @param[in] instruction where variable is assigned
 */

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
    for(auto& [key, val] : searching){
         if(instruction.find(key) != string::npos){
             return key;
         }
    }
     return "not here";
   
}

/**
 * Saves assigned variables in a vector   
 *
 * @param[in] instruction where variable is assigned
 */

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
    string cicleTimes= totalComplexity.at(line).complexity;
    if(instruction.find("{") != string::npos){
          ciclos.push(cicleTimes);
    }else{
        ciclePen= cicleTimes;
    }
    totalComplexity.at(line).complexity = " "+to_string(elemental)+totalComplexity.at(line).complexity;
    totalComplexity.at(line).complexity = totalComplexity.at(line).complexity+"+1";
    }

/**
 * Saves assigned variables in a vector   
 *
 * @param[in] instruction where variable is assigned
 */

    void addWhile(string instruction, string variable){
        string incrementing = variable;
        int whileIndex = searching[variable];
        string initializing;
        searching.erase(variable);
         for(auto const& pair: searching){
         if(whileIndex == pair.second){
             initializing= pair.first;
         }
        }
        cout<<instruction<<" this is the instruction "<<endl;
        searching.erase(initializing);
        if(instruction.find("/=") != string::npos){
            totalComplexity.at(whileIndex).complexity = "log"+findIncrement(instruction)+"("+initializing+"-"+variable+")";
        }
        else if(instruction.find("*=") != string::npos){
            totalComplexity.at(whileIndex).complexity = "log"+findIncrement(instruction)+"("+initializing+"-"+variable+")";

        }
        else if(instruction.find("/") != string::npos){
            totalComplexity.at(whileIndex).complexity = "log"+findIncrement(instruction)+"("+initializing+"-"+variable+")";
        }
        else if(instruction.find("*") != string::npos){
            totalComplexity.at(whileIndex).complexity = "log"+findIncrement(instruction)+"("+initializing+"-"+variable+")";

        }else if(instruction.find("++") != string::npos){
            totalComplexity.at(whileIndex).complexity = variable+"-"+initializing;
        }else if(instruction.find("+=") != string::npos){
            totalComplexity.at(whileIndex).complexity = variable+"-"+initializing+ "* 1/"+findIncrement(instruction);
        }else if(instruction.find("--") != string::npos){
            totalComplexity.at(whileIndex).complexity = initializing+"-"+variable;
        }else if(instruction.find("-=") != string::npos){
            totalComplexity.at(whileIndex).complexity = initializing+"-"+variable+ "* 1/"+findIncrement(instruction);
        }else{
           totalComplexity.at(whileIndex).complexity = "non of the above";  
        }

    }

/**
 * Saves assigned variables in a vector   
 *
 * @param[in] instruction where variable is assigned
 */
    int analyzeIF(string instruction){
        string str = instruction; // instruccion a analizar (en este caso IF)

        int posParentesis = str.find('(');
        int posParentesis2 = str.find(')');
  
        string str2 = str.substr(posParentesis+1, (posParentesis2-1)-posParentesis);
        return countElemental(str2);
    }

/**
 * Saves assigned variables in a vector   
 *
 * @param[in] instruction where variable is assigned
 */
    int analyzePrint(string instruction){
        string sub1 = "";
        string sub2 = "";
        string current = instruction;
        vector<string> varia;
        int finalComplex = 0;
        string varAcum;
        

        while(current.find("\"") != string::npos){
            sub1= current.substr(current.find("\"")+1);
            sub1 = sub1.substr(sub1.find("\"")+1);
            sub2 = current.substr(0, current.find("\""));
            current = sub2+sub1;
        }
        if(current.find("cout") != string::npos){
            current = current.substr(4, current.length()-5);
            if(current.find("endl") != string::npos){
                current = current.substr(0, current.find("endl"));
            }
        }
        if(current.find("printf") != string::npos){
            current = current.substr(current.find("printf")+7, current.find(")")-current.find("return")-8);
        }

        if(current.find("return") != string::npos){
            current = current.substr(current.find("return")+6, current.find(";")-current.find("return")-6);
        }


        for(int i=0; i<current.length(); i++){
            if(current[i]== '-' || current[i]== '*' || current[i]== '+' || current[i]== '/' || current[i]== '='  || current[i]== '['){
                finalComplex++;
                if(!varAcum.empty()){
                    varia.push_back(varAcum);
                    varAcum ="";
                }
            }
            else if(current[i]== ',' || current[i]== ' ' || current[i]== '+' || current[i]== '<'){
                if(!varAcum.empty()){
                     varia.push_back(varAcum);
                    varAcum ="";
                }
            }else if(!isdigit(current[i])){
                varAcum+=current[i];
            }
            if(i==current.length()-1){
                if(!varAcum.empty()){
                    varia.push_back(varAcum);
                    varAcum ="";
                }
            }


           
        }
        return finalComplex+varia.size();

    }


   void analyzeComplexity(){
    for (int i=0; i<counterLines; i++){
            string current = totalComplexity.at(i).instruction;

            if(identifyInstruction(current)< 3 && identifyInstruction(current) != -1){
                totalComplexity.at(i).complexity= to_string(analyzePrint(current));
            }
            else if(identifyInstruction(current) == 3)
            {

                totalComplexity.at(i).setComplexity(to_string(analyzeIF(current)), analyzeIF(current));
                ciclos.push(" ");

            }else if(identifyInstruction(current) == 4)
            {

                totalComplexity.at(i).complexity = analyzeFor(current);
                ciclos.push(cicleTimes(current));
            }else if(identifyInstruction(current) == 5)
            {

                analyzeWhile(current, i);
            }else{
                if(current.find("{") != string::npos){
                    if(!ciclePen.empty()){
                        ciclos.push(ciclePen);
                        ciclePen ="";
                    }else{
                        ciclos.push(" ");

                    }
                }
                int contElemental = countElemental(current);               
                totalComplexity.at(i).setComplexity(to_string(contElemental), contElemental);
            }

            if(current.find("}") != string::npos){
                ciclos.pop();
            }
            
             if(!ciclos.empty()){
                stack<string> temp;
                 while(!ciclos.empty()){
                    temp.push(ciclos.top());
                    if(ciclos.top() != " "){
                    totalComplexity.at(i).cycle += " ("+ciclos.top()+") ";
                    }
                    ciclos.pop();

                 }
                 while(!temp.empty()){
                     ciclos.push(temp.top());
                     temp.pop();
                 }
             }

            
        }
       
   }

  

    void printComplexity(){
         totalComplexity.at(0).printTitles();
    for(int i=0; i<totalComplexity.size(); i++){
        (totalComplexity.at(i)).printInstruction();
    }  
    }

    void calculateFinalComplexity(){
        vector<string> finalComplex;
        string final;
        for(int i=0; i<totalComplexity.size(); i++){
            string current = totalComplexity.at(i).instruction;
            if(current.find("while") != string::npos || current.find("for") != string::npos){
                finalComplex.push_back(totalComplexity.at(i).complexity);
            }else{
                if(totalComplexity.at(i).eo!=0){
                    finalComplex.push_back(totalComplexity.at(i).complexity+"*("+totalComplexity.at(i).cycle+")");
                }
            }
        } 

        for(int i=0; i<finalComplex.size(); i++){
            if(i==finalComplex.size()-1){
                final+=finalComplex.at(i);
            }else{
                final+=finalComplex.at(i)+"+ ";
            }
        }
        

        cout<<"T(n)= "<<endl;
        cout<<final<<endl;
        writeFile(final);
        system("pip install sympy");
        system("python3 poly.py");
    }
   
   int writeFile (string towrite) 
{
  ofstream myfile;
  cout<<"writing"<<endl;
  myfile.open ("example.txt");
  myfile << towrite;
  myfile.close();
  return 0;
}


};
int main(int argc, char* argv[])
{
    
   Analyzer<string, int> a;
   a.readFile(argc, argv);
   a.analyzeComplexity();
   a.printComplexity();
   a.calculateFinalComplexity();
}
