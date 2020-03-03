
#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include <iomanip>
#include <set> 
#include <stack>
#include <map>
#include <sstream>
#include <cctype>


using namespace std;


//clase instrucción

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
    cout << '|' << setw(width) << t << '|' << setw(width) << t2 << '|' << setw(width) << t3 << '|' << setw(width) << t4 << '|' << setw(width) << t5 << '|'<<endl;

    
    }

    void printInstruction(){
        string eo_s= to_string(eo);
        string no_s= to_string(noLine);
        printElement(no_s, instruction.substr(0,30), cycle, complexity, eo_s, 30);
    }

    void printTitles(){
        printElement("no", "instrucción", "ciclo", "complejidad", "OE", 30);
    }
    void setComplexity(string complexity, int eo){
        this->complexity = complexity;
        this->eo = eo;
    }

};

class Polynome{
    public:
    map<string, int> variables;

    void add(string variable, int value){
        variables.insert(pair<string, int>(variable,value));
    }

    Polynome(){}

    void addPoly(string polinome){
        string acumulatedKey;
        string acumulatedValue;
        for(int i=0; i<polinome.length(); i++){
            if(polinome[i] == '+' || polinome[i] == '-' || i==polinome.length()-1){
                
                    if(i==polinome.length()-1){
                        if(!isdigit(polinome[i])){
                            acumulatedKey+=polinome[i];
                        }
                        else if(isdigit(polinome[i])){
                            acumulatedValue+=polinome[i];
                        }else if(polinome[i]=='-'){
                            acumulatedValue="-"+acumulatedValue;
                        }
                       
                    }
                     if(acumulatedValue.empty()){
                        acumulatedValue= "1";
                    }
                    if(acumulatedKey.empty()){
                        acumulatedKey= "const";
                    }

                    cout<<"acumulated Key"<<acumulatedKey<<" acumulated Value "<< acumulatedValue<<endl;

                    if(variables.count(acumulatedKey) == 0){
                    variables.insert(pair<string, int> (acumulatedKey, stoi(acumulatedValue)));
                    }else{
                    variables[acumulatedKey]+=stoi(acumulatedValue);
                    }
                acumulatedKey="";
                acumulatedValue="";
            }
            else if(!isdigit(polinome[i])){
                acumulatedKey+=polinome[i];
            }
            else if(isdigit(polinome[i])){
                acumulatedValue+=polinome[i];
            }
          
        }
    }

    void printPoly(){
    for (map<string, int>::iterator iter = variables.begin(); iter != variables.end(); iter++)
        {
    cout << "Key: " << iter->first << endl << "Values:" << iter->second<<endl;
    }
    }
};
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
    string identifiersInst[] = { "cout", "printf", "return", "if", "for", "while", "else"};

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
    string increm= "abcd";
    if(increment.find("=") != string::npos){
        return increment.substr(increment.find("=")+1,(increment.find(")")-4));
    }else if(increment.find("++") != string::npos || increment.find("--") != string::npos){
        return "";
    }
     return "";
}

string cicleTimes(string instruction){
    vector<string> partsOfFor= split(instruction, ';');

    string comparison = partsOfFor.at(1);
    string initialized = partsOfFor.at(0);
    string increment = partsOfFor.at(2);
    Polynome times;
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
    return to_string(a)+"+"+to_string(b)+"*("+cycle_temp+")+"+to_string(c)+"*("+cycle_temp+")";
    
}

// string checkPrint(string instruction){
//     bool insideQuotations =false;
//     int saveStart;
//     for(int i=0; i<instruction.length(); i++){
//         if(instruction[i]=='/" '){
//             if(!insideQuotations){
//                 insideQuotations = true;
//                 saveStart = i;
//             }else{
//                 insideQuotations = false;
//                 saveEnd =  i;
//             }
//         }
//     }
// }


int main(int argc, char* argv[])
{
    vector<Instruction> totalComplexity;
    string x = "";
    ifstream* inFile;
    int counterLines = 0;
    stack<string> ciclos;

   
   
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

        for (int i=0; i<counterLines; i++){
            string current = totalComplexity.at(i).instruction;
            if(identifyInstruction(current) == 4){
                totalComplexity.at(i).complexity = analyzeFor(current);
                ciclos.push(cicleTimes(current));
            }else if(identifyInstruction(current) < 2){
                
            }
            else{
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
    //
    //print table
    totalComplexity.at(0).printTitles();
    for(int i=0; i<totalComplexity.size(); i++){
        (totalComplexity.at(i)).printInstruction();
    }   

   

    cout<<"ciclos"<<endl;
    while(!ciclos.empty()){
        cout<<ciclos.top()<<endl;
        ciclos.pop();
    }  

  


}
