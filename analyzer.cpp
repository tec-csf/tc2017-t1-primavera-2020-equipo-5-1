
/*
 * This file is part of {{ tc2017-t1-primavera-2020-equipo-5-1 }}.
 *
 * Developed for Diseño, Análisis de Algoritmos
 * This product includes software developed by the LSST Project
 * (https://www.lsst.org).
 * See the COPYRIGHT file at the top-level directory of this distribution
 * for details of code ownership.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
 * 
 * @note although this is a generic class the implementation only applies for <string, int> types. 
 */

template <class T, class I>
class Analyzer{
        public:
        vector<Instruction> programLines;
        T read = "";
        ifstream* inFile;
        I counterLines = 0;
        stack<T> ciclos;
        T ciclePen;
        map<T ,T> assignedVariables;
        map<T, I> searchingVariables;



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
            T complete ="";
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
 * @param[out] inderead of the instruction to be analyzed
 * @param[in] instruction as a string, where reserved words will be searched for
 */

        int identifyInstruction(string instruction){
            string identifiersInst[] = { "cout", "printf", "return", "if", "for", "while", "main", "else"};

            for(int i=0; i<7; i++)
            {
                if(instruction.find(identifiersInst[i]) != string::npos)
                {
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

        string findTypeComparison(string comparison, int operation)
        {
            if(operation == 1){
                if(comparison.find("<=") != string::npos)
                {
                    return comparison.substr(comparison.find("=")+1);
                }
                if(comparison.find("<") != string::npos)
                {
                    return comparison.substr(comparison.find("<")+1);
                }
                if(comparison.find(">") != string::npos)
                {
                    return comparison.substr(0, comparison.find(">"));
                }
            }else if(operation == -1)
            {
                if(comparison.find(">=") != string::npos)
                {
                    return comparison.substr(comparison.find("=")+1);
                }
                if(comparison.find(">") != string::npos)
                {
                    return comparison.substr(comparison.find(">")+1);
                }
                if(comparison.find("<") != string::npos)
                {
                    return comparison.substr(0, comparison.find("<"));
                }
            }
            return "not found";
        }

/**
 * Find the factor of incrementation in a for loop or while loop.
 *
 * @param[out] sting with the incrementing factor
 * @param[in] string that has the incrementing instruction
 */

        string findIncrement(string increment)
        {
            if(increment.find("+=") != string::npos || increment.find("-=") != string::npos || increment.find("*=") != string::npos || increment.find("/=") != string::npos){
                if(increment.find(")") != string::npos)
                {
                    return increment.substr(increment.find("=")+1, increment.find(")")-increment.find("=")-1);
                }else
                {
                    return increment.substr(increment.find("=")+1, increment.find(";")-increment.find("=")-1);

                }
            }
            else if(increment.find("++") != string::npos || increment.find("--") != string::npos)
            {
                return "";
            }else if(increment.find("+") != string::npos)
            {
                if(increment.find(")") != string::npos)
                {
                    return increment.substr(increment.find("+")+1, increment.find(")")-increment.find("+")-1);
                }else
                {
                    return increment.substr(increment.find("+")+1, increment.find(";")-increment.find("+")-1);
                }
            }else if(increment.find("-") != string::npos)
            {
                if(increment.find(")") != string::npos)
                {
                    return increment.substr(increment.find("-")+1, increment.find(")")-increment.find("-")-1);
                }else
                {
                    return increment.substr(increment.find("")+1, increment.find(";")-increment.find("-")-1);
                }
            }else if(increment.find("*") != string::npos)
            {
                if(increment.find(")") != string::npos)
                {
                    return increment.substr(increment.find("*")+1, increment.find(")")-increment.find("*")-1);
                }else
                {
                    return increment.substr(increment.find("*")+1, increment.find(";")-increment.find("*")-1);
                }
            }else if(increment.find("/") != string::npos)
            {
                if(increment.find(")") != string::npos)
                {
                    return increment.substr(increment.find("/")+1, increment.find(")")-increment.find("/")-1);
                }else
                {
                    return increment.substr(increment.find("/")+1, increment.find(";")-increment.find("/")-1);
                }
            }else
            {
                return "";
            }
        }

/**
 * Calculates the amount of times a cicle will be run 
 *
 * @param[out] string with amount of times cicle will be run 
 * @param[in] cicle in the form of an instruction
 */

        T cicleTimes(T instruction){
            T comparison;
            T initialized;
            T increment;
            vector<T> partsOfFor;

            partsOfFor = split(instruction, ';');
            comparison = partsOfFor.at(1);
            initialized = partsOfFor.at(0);
            increment = partsOfFor.at(2);

            if(increment.find("++") != string::npos)
            {
                return (findTypeComparison(comparison, 1)+"-"+initialized.substr(initialized.find("=")+1));
            }
            if(increment.find("--") != string::npos)
            {
                return (initialized.substr(initialized.find("=")+1)+"-"+findTypeComparison(comparison, -1));
            }
            if(increment.find("+") != string::npos)
            {
                return findTypeComparison(comparison, 1)+"-"+initialized.substr(initialized.find("=")+1)+"* "+"(1/"+findIncrement(increment)+")";
            }
            if(increment.find("-") != string::npos){
                return initialized.substr(initialized.find("=")+1)+"-"+findTypeComparison(comparison, -1)+"*(1/"+findIncrement(increment)+")";
            }
            if(increment.find("*=") != string::npos)
            {
                return " log"+increment.substr(increment.find("=")+1, increment.find(")")-increment.find("=")-1)+"("+findTypeComparison(comparison, 1)+"-"+initialized.substr(initialized.find("=")+1)+")";
            }
            if(increment.find("/=") != string::npos)
            {
                return " log"+increment.substr(increment.find("=")+1, increment.find(")")-increment.find("=")-1)+"("+initialized.substr(initialized.find("=")+1)+"-"+findTypeComparison(comparison, -1)+")";
            }
            if(increment.find("*") != string::npos)
            {
                return " log"+increment.substr(increment.find("*")+1, increment.find(")")-increment.find("*")-1)+"("+findTypeComparison(comparison, 1)+"-"+initialized.substr(initialized.find("=")+1)+")";
            }
            if(increment.find("/") != string::npos)
            {
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
            int num = 0;
            string temps= "";
            char tmp[] = {'+', '*', '=', '-', '/', '%', '[', '<', '>', ','};
            string tmpdouble[] = {"++", "--", ">=", "!=", "==", "<=", "&&", "||"};
            set<char> operators(tmp, tmp+ (sizeof(tmp) / sizeof(tmp[0])));
            set<string> doubleOp(tmpdouble, tmpdouble+ (sizeof(tmpdouble) / sizeof(tmpdouble[0])));

            for(int i=0; i<instruction.length(); i++)
            {
                
                temps.append(string(1,instruction[i])); 
                temps.append(string(1,instruction[i+1]));

                if(doubleOp.count(temps) !=0)
                {
                    i++;
                    num++;
                }
                else if(operators.count(instruction[i])!=0)
                {
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
                T cycleTimes;
                T comparison;
                T initialized;
                T increment;

                vector<string> partsOfFor= split(instruction, ';');

                comparison = partsOfFor.at(1);
                initialized = partsOfFor.at(0);
                increment = partsOfFor.at(2);

                int iIntialized = countElemental(initialized);
                int iComparison = countElemental(comparison);
                int iIncrement = countElemental(increment);

                
                cycleTimes= cicleTimes(instruction);
                
                return to_string(iIntialized)+"+"+to_string(iComparison)+"*("+cicleTimes(instruction)+")+"+to_string(iIncrement)+"*("+cicleTimes(instruction)+"+1)";
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
                    while(getline(*inFile, read))
                    {
                        Instruction* temp = new Instruction(read.c_str(), counterLines);
                        programLines.push_back(*temp);
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
            string currentInstruction="";
            string key = "";
            string element = "";
            vector<string> instructionVector= split(instruction, ',');

            if(instruction.find("int") != string::npos || instruction.find("float") != string::npos || instruction.find("double") != string::npos)
            {
                for(int i=0; i<instructionVector.size(); i++)
                {
                    currentInstruction = instructionVector.at(i);
                    if(currentInstruction.find("=") != string::npos)
                    {
                        if(currentInstruction.find("int") != string::npos || currentInstruction.find("float") != string::npos || currentInstruction.find("double") != string::npos)
                        {
                            key = currentInstruction.substr(currentInstruction.find(" "), currentInstruction.find("=")-3);
                        }else
                        {
                            key = currentInstruction.substr(0, currentInstruction.find("="));
                        }
                        element = currentInstruction.substr(currentInstruction.find("=")+1);
                        assignedVariables.insert(pair<string, string> (key, element));
                    }
                }
            }
        }

/**
 * check if the current line is increment for while loop
 *
 * @param[in] the instruction that is analyzed
 * @param[out] the instruction that contains incremnt in while loop
 */


        string checkWhile(string instruction){
            for(auto& [key, val] : searchingVariables){
                if(instruction.find(key) != string::npos){
                    return key;
                }
            }
            return "not here";
        
        }

/**
 * Analyzes while loop 
 *
 * @param[in] the instruction in which the while loop is found 
 * @param[in] the line in which the while loop is found 
 */

        void analyzeWhile(string instruction, int line)
        {
            int elemental;
            int saveIndex;
            int offset=0;
            string beforeCompare;
            string afterCompare;
            string symbols[] = {"<", ">", "<=",">=","==","!="};
            string delimeter ="";
            string currentLine;
            string temporaryWhile;

            elemental = countElemental(instruction.substr(instruction.find("("), instruction.find("(")-1));

                for(int i= 0; i<6; i++)
                {
                    if(instruction.find(symbols[i])!= string::npos)
                    {
                        delimeter= symbols[i];
                        saveIndex = i;
                    }
                }
               
                if(saveIndex>1)
                {
                    offset=1;
                }

                beforeCompare = instruction.substr(instruction.find("(")+1, instruction.find(delimeter)-instruction.find("(")-1);
                afterCompare = instruction.substr(instruction.find(delimeter)+1+offset,instruction.find(")")-instruction.find(delimeter)-offset-1);
                searchingVariables.insert(pair<string, int> (beforeCompare, line));
                searchingVariables.insert(pair<string, int> (afterCompare, line));
                
                for(int i= line+1; i<programLines.size(); i++)
                {
                    currentLine= programLines.at(i).instruction;
                    if(currentLine != "}")
                    {
                        temporaryWhile = checkWhile(currentLine);
                            if(temporaryWhile != "not here")
                            {
                                addWhile(currentLine, temporaryWhile);
                            }
                    }
                }
           
            ciclePen =  programLines.at(line).complexity;

            programLines.at(line).complexity = to_string(elemental)+"*"+programLines.at(line).complexity;
            programLines.at(line).complexity = programLines.at(line).complexity+"+1";
        }

/**
 * If it finds the variable increment within the while loop it will return the amount of times the variable is incremented
 *
 * @param[in] instruction where variable is assigned
 * @param[in] variable found in the increment
 * 
 */

            void addWhile(string instruction, string variable)
            {
                string incrementing = variable;
                int whileInderead = searchingVariables[variable];
                string initializing;

                searchingVariables.erase(variable);

                for(auto const& pair: searchingVariables)
                {
                    if(whileInderead == pair.second)
                    {
                        initializing= pair.first;
                    }
                }

                searchingVariables.erase(initializing);

                if(instruction.find("/=") != string::npos)
                {
                    programLines.at(whileInderead).complexity = "log"+findIncrement(instruction)+"("+initializing+"-"+variable+")";
                }
                else if(instruction.find("*=") != string::npos)
                {
                    programLines.at(whileInderead).complexity = "log"+findIncrement(instruction)+"("+initializing+"-"+variable+")";
                }
                else if(instruction.find("/") != string::npos)
                {
                    programLines.at(whileInderead).complexity = "log"+findIncrement(instruction)+"("+initializing+"-"+variable+")";
                }
                else if(instruction.find("*") != string::npos)
                {
                    programLines.at(whileInderead).complexity = "log"+findIncrement(instruction)+"("+initializing+"-"+variable+")";

                }else if(instruction.find("++") != string::npos)
                {
                    programLines.at(whileInderead).complexity = variable+"-"+initializing;
                }else if(instruction.find("+=") != string::npos)
                {
                    programLines.at(whileInderead).complexity = variable+"-"+initializing+ "* 1/"+findIncrement(instruction);
                }else if(instruction.find("--") != string::npos)
                {
                    programLines.at(whileInderead).complexity = initializing+"-"+variable;
                }else if(instruction.find("-=") != string::npos)
                {
                    programLines.at(whileInderead).complexity = initializing+"-"+variable+ "* 1/"+findIncrement(instruction);
                }else
                {
                programLines.at(whileInderead).complexity = "non of the above";  
                }

            }

/**
 * Analyzes the amount of elemental operations in an if statement
 *
 * @param[in] instruction where the if statement is found 
 * @param[out] the amount of elemental operations in the if statement
 */

            int analyzeIF(string instruction){
                string str = instruction; // instruccion a analizar (en este caso IF)

                int posParentesis = str.find('(');
                int posParentesis2 = str.find(')');
        
                string str2 = str.substr(posParentesis+1, (posParentesis2-1)-posParentesis);
                return countElemental(str2);
            }

/**
 * Analyzes the complexity of a print instruction including cout, printf, and reutrns
 *
 * @param[in] the isntruction to be analizes
 * @param[out] the amount of elemental operations found according to the amount of times a variable  is retrieved
 */
            int analyzePrint(string instruction){
                string subBeforeQuotations = "";
                string subAfterQuotations = "";
                string currentInstruction = instruction;
                vector<string> varia;
                int finalCompleread = 0;
                string varAcum;

                while(currentInstruction.find("\"") != string::npos)
                {
                    subBeforeQuotations= currentInstruction.substr(currentInstruction.find("\"")+1);
                    subBeforeQuotations = subBeforeQuotations.substr(subBeforeQuotations.find("\"")+1);
                    subAfterQuotations = currentInstruction.substr(0, currentInstruction.find("\""));
                    currentInstruction = subAfterQuotations+subBeforeQuotations;
                }

                if(currentInstruction.find("cout") != string::npos)
                {
                    currentInstruction = currentInstruction.substr(4, currentInstruction.length()-5);
                    if(currentInstruction.find("endl") != string::npos)
                    {
                        currentInstruction = currentInstruction.substr(0, currentInstruction.find("endl"));
                    }
                }

                if(currentInstruction.find("printf") != string::npos)
                {
                    currentInstruction = currentInstruction.substr(currentInstruction.find("printf")+7, currentInstruction.find(")")-currentInstruction.find("return")-8);
                }

                if(currentInstruction.find("return") != string::npos)
                {
                    currentInstruction = currentInstruction.substr(currentInstruction.find("return")+6, currentInstruction.find(";")-currentInstruction.find("return")-6);
                }


                for(int i=0; i<currentInstruction.length(); i++){
                    if(currentInstruction[i]== '-' || currentInstruction[i]== '*' || currentInstruction[i]== '+' || currentInstruction[i]== '/' || currentInstruction[i]== '='  || currentInstruction[i]== '['){
                        finalCompleread++;

                        if(!varAcum.empty())
                        {
                            varia.push_back(varAcum);
                            varAcum ="";
                        }
                    }
                    else if(currentInstruction[i]== ',' || currentInstruction[i]== ' ' || currentInstruction[i]== '+' || currentInstruction[i]== '<'){
                        if(!varAcum.empty())
                        {
                            varia.push_back(varAcum);
                            varAcum ="";
                        }
                    }else if(!isdigit(currentInstruction[i]))
                    {
                        varAcum+=currentInstruction[i];
                    }
                    if(i==currentInstruction.length()-1)
                    {
                        if(!varAcum.empty())
                        {
                            varia.push_back(varAcum);
                            varAcum ="";
                        }
                    }
                
                }
                return finalCompleread+varia.size();

            }
/**
 * Iterates through the vector of objects type Instruction and saves the final variable in as the complexity of the given function in the vector 
 *
 */

        void analyzecomplexity()
        {
            string currentInstruction = "";
            int contElemental = 0;
            stack<string> temporalStack;

            for (int i=0; i<counterLines; i++){
                    currentInstruction = programLines.at(i).instruction;

                    if(identifyInstruction(currentInstruction)< 3 && identifyInstruction(currentInstruction) != -1)//PRINT & RETURN
                    {
                        programLines.at(i).complexity= to_string(analyzePrint(currentInstruction));
                    }
                    else if(identifyInstruction(currentInstruction) == 3)//IF 
                    {
                        programLines.at(i).setComplexity(to_string(analyzeIF(currentInstruction)), analyzeIF(currentInstruction));
                        ciclos.push(" ");

                    }else if(identifyInstruction(currentInstruction) == 4)
                    {
                        programLines.at(i).complexity = analyzeFor(currentInstruction);
                        ciclos.push(cicleTimes(currentInstruction));
                    }
                    else if(identifyInstruction(currentInstruction) == 5)
                    {

                        analyzeWhile(currentInstruction, i);
                    }else
                    {
                        if(!ciclePen.empty())
                        {
                            ciclos.push(ciclePen);
                            ciclePen ="";
                        }
                        if(currentInstruction.find("{") != string::npos)
                        {
                            ciclos.push(" ");
                        }

                    contElemental = countElemental(currentInstruction);               
                    programLines.at(i).setComplexity(to_string(contElemental), contElemental);
                    }
   
                    

                    if(currentInstruction.find("}") != string::npos)
                    {
                        ciclos.pop();
                    }
                    
                    if(!ciclos.empty())
                    {
                        while(!ciclos.empty())
                        {
                            temporalStack.push(ciclos.top());
                            if(ciclos.top() != " ")
                            {
                                programLines.at(i).cycle += "*("+ciclos.top()+")";
                            }
                            ciclos.pop();

                        }
                        while(!temporalStack.empty())
                        {
                            ciclos.push(temporalStack.top());
                            temporalStack.pop();
                        }
                    }

                    
            }
            
        }

        
/**
 * Prints complexity in table format
 *  @note bigger lines might cause table to change width
 */

            void printcomplexity()
            {
                programLines.at(0).printTitles();
                for(int i=0; i<programLines.size(); i++)
                {
                    (programLines.at(i)).printInstruction();
                }  
            }

/**
 * Calculates final complexity and prints it
 * 
 */
            void calculateFinalcomplexity()
            {
                vector<string> finalCompleread;
                string final;
                string currentInstruction;

                for(int i=0; i<programLines.size(); i++)
                {
                    currentInstruction = programLines.at(i).instruction;
                    if(currentInstruction.find("while") != string::npos || currentInstruction.find("for") != string::npos)
                    {
                        finalCompleread.push_back(programLines.at(i).complexity+"*("+programLines.at(i).cycle+")");
                    }else
                    {
                        if(programLines.at(i).eo!=0)
                        {
                            finalCompleread.push_back(programLines.at(i).complexity+"*("+programLines.at(i).cycle+")");
                        }
                    }
                } 

                for(int i=0; i<finalCompleread.size(); i++)
                {
                    if(i==finalCompleread.size()-1)
                    {
                        final+=finalCompleread.at(i);
                    }else
                    {
                        final+=finalCompleread.at(i)+"+";
                    }
                }
                cout<<"T(n)= "<<endl;
                cout<<final<<endl;
                writeFile(final);
                system("pip install sympy");
                system("python poly.py");

                
            }

/**
 * Writes in file called example.text
 * 
 */
   
        int writeFile (string towrite) 
        {
            ofstream myfile;

            myfile.open ("example.txt");
            myfile << towrite;
            myfile.close();
            return 0;
        }


};



