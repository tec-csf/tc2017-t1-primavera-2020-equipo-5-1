
#include <iostream> 
#include <cstring>
#include <string>
#include <map>
#include <vector>
#include <sstream>


using namespace std;

class Variable{
    public:
        int potencia;
        int coef;
        string variable;

    void set(int potencia, int coef, string variable){
        this->variable = variable;
        this->coef = coef;
        this->potencia = potencia;
    }
};

class Mono{
    public:
    vector<Variable> variables;

    void addVar(int potencia, int coef, string variable){
        Variable a;
        a.set(potencia, coef, variable);
        variables.push_back(a);
    }
    
    
    void turnToMonome(string mono){
        string acumCoef = "";
        string acumVar = "";
        string acumPot = "";
        bool getPot = false;
        for(int i=0; i<mono.length(); i++){
            if(isdigit(mono[i]) || mono[i]=='-' ){
                if(getPot){
                    acumPot += mono[i];
                }else{
                    acumCoef+=mono[i];
                }
            }else{
                if(mono[i]=='*'){
                    getPot = true;
                }else{
                    if(getPot || mono[i]=='.'){
                    if(acumPot.empty() && (!acumVar.empty())){
                        acumPot="1";
                    }
                    if(acumPot.empty()){
                        acumPot="0";
                    }
                    if(acumVar.empty()){
                        acumVar="const";
                    }
                 
                    if(acumCoef.empty()){
                        acumCoef="1";
                    }
                    if(acumCoef!="0"){
                        addVar(stoi(acumPot), stoi(acumCoef), acumVar);
                        getPot=false;
                        acumPot="";
                        acumVar="";
                        acumCoef="";
                    
                    }
                    }
                    if(mono[i]!='.'){
                         acumVar+=mono[i];

                    }


                }
            }

            if(i== mono.length()-1){
                    
                    if(acumPot.empty() && !acumVar.empty()){
                        acumPot="1";
                    }

                    if(acumPot.empty()){
                        acumPot="0";
                    }
                    if(acumVar.empty()){
                        acumVar="const";
                    }
                    if(acumCoef.empty()){
                        acumCoef="1";
                    }
                    if(acumCoef!="0"){

                        addVar(stoi(acumPot), stoi(acumCoef), acumVar);
                        getPot=false;
                        acumPot="";
                        acumVar="";
                        acumCoef="";
                    }
                    
            }
        }
        
        
    }  

    void printMono(){
        for(int i=0; i<variables.size(); i++){
            cout<<"potencia "<<variables.at(i).potencia<<endl;
            cout<<"variable "<<variables.at(i).variable<<endl;
            cout<<"coeficiente "<<variables.at(i).coef<<endl;

        }
    } 
    
     int findVar(string var){
        for(int i=0; i<variables.size(); i++){
            if(strcmp(variables.at(i).variable.c_str(), var.c_str())==0){
                return i;
            }
        }
        return -1;
    }

    
};

class Poly{
    public:
    vector<string> monomes;
    map<string, int> variab;

    void flush(){
        monomes.clear();
        variab.clear();
    }


    string simplify(string instruction){
        string acumVar="";
        string acumCoef="";
        string final;
        bool varStart = false;
        for(int i=0; i<instruction.length(); i++){
            if(instruction[i]=='+' || instruction[i]=='-' || i== instruction.length()-1){
                if(i== instruction.length()-1){
                    if(isdigit(instruction[i])){
                        acumCoef+=instruction[i];
                    }else{
                        if(instruction[i]!= ' '){
                            acumVar+=instruction[i]; 
                        }
                    }
                }
                if(acumCoef.empty()){
                    acumCoef = "1";
                }
                if(acumVar.empty()){
                    acumVar = "const";
                }
                if(acumCoef!="0"){
                    if(acumCoef=="-"){
                        acumCoef ="-1";
                    }
                    addPoly(acumVar, stoi(acumCoef));
                }
                if(instruction[i]=='-'){
                    acumCoef="-";
                }else{
                    acumCoef="";
                }
                varStart = false;
                acumVar="";
            }
            else if(isdigit(instruction[i]) && !varStart){
                acumCoef+=instruction[i];
            }else{
                if(instruction[i]!= ' '){
                acumVar+=instruction[i];
                }
            }
        }

            for( auto const& [key, val] : variab )
            {
                string printKey;
                string symbol;
                string printVal;
                printVal= to_string(val);
                if(key =="const"){
                    printKey ="";
                }else{
                    printKey =key;
                    if(val==-1){
                        printVal="-";
                    }
                    if(val==1 && !key.empty()){
                        printVal="";
                    }
                }
                if(val>0){
                    symbol = "+";
                }
                final+=symbol+printVal+printKey;
            }
            if(final[0]=='+'){
                final= final.substr(final.find('+')+1);
            }
            return final;

    }


    void addPoly(string var, int coef){
        if(variab.find(var) != variab.end()){
            variab[var]+=coef;
            if(variab[var]==0){
                variab.erase(var);
            }
        }else{
            variab.insert(pair<string, int> (var, coef));
        }
    }

    string mono2str(Mono m){
        string final="";
        string printCoef;
        string printPot;
        string printVar;
        for(int i=0; i<m.variables.size(); i++){
            Variable currentVar= m.variables.at(i);
            if(currentVar.coef ==1){
                printCoef="";
            }else{
                printCoef=to_string(currentVar.coef);
            }

            if(currentVar.potencia==1 || currentVar.potencia==0){
                printPot="";
            }else{
                printPot="*"+to_string(currentVar.potencia);
            }
            if(currentVar.variable == "const"){
                printVar="";
            }else{
                printVar= currentVar.variable;
            }
            final+=printCoef+printVar+printPot;
        }
        return final;
    }

    string multiplyMono(string a, string b){
        Mono mono_a;
        mono_a.turnToMonome(a);
        Mono mono_b;

        mono_b.turnToMonome(b);

        Mono final;

        for(int i=0; i<mono_a.variables.size(); i++){
            Variable current= mono_a.variables.at(i);
            if(final.findVar(current.variable) == -1){
                final.addVar( current.potencia, 1, current.variable);
            }else{
                int index = final.findVar(current.variable);
                final.variables.at(index).potencia+=current.potencia;
            }
            final.variables.at(0).coef*= current.coef;

        }
        for(int i=0; i<mono_b.variables.size(); i++){
            Variable current= mono_b.variables.at(i);
            if(final.findVar(current.variable) == -1){
                final.addVar(current.potencia, 1, current.variable);
            }else{
                int index = final.findVar(current.variable);
                final.variables.at(index).potencia+=current.potencia;
            }
            final.variables.at(0).coef*= current.coef;
        }

        return mono2str(final);

    }

    string multiply(string a, string b){
        string acumMono;
        Poly pola;
        Poly polb;
        string final ="";
        for(int i=0; i<a.length(); i++){
            if(a[i]=='+' || a[i]=='-'){
                if(acumMono != "0"){
                    pola.monomes.push_back(acumMono);
                }
                 if(a[i]=='-'){
                        acumMono="-";
                }else{
                    acumMono="";
                }
            }else if(i==a.length()-1){
                acumMono+=a[i];
                if(acumMono != "0"){
                    pola.monomes.push_back(acumMono);
                    
                }
                if(a[i]=='-'){
                        acumMono="-";
                }else{
                    acumMono="";
                }
            }else{
                acumMono+=a[i];  
            }
        }

        for(int i=0; i<b.length(); i++){
            if(b[i]=='+' || b[i]=='-' ){
                if(acumMono != "0"){
                polb.monomes.push_back(acumMono);
                }
                if(b[i]=='-'){
                        acumMono="-";
                }else{
                    acumMono="";
                }
            }else if( i==b.length()-1){
                acumMono+=b[i];
                if(acumMono != "0"){
                polb.monomes.push_back(acumMono);
                }
                if(b[i]=='-'){
                        acumMono="-";
                }else{
                    acumMono="";
                }
            }else{
                acumMono+=b[i];  
            }
        }

        for(int i=0; i<pola.monomes.size(); i++){
            for(int j=0; j<polb.monomes.size(); j++){
                string result = multiplyMono(pola.monomes.at(i), polb.monomes.at(j));
                if(result[0]== '-' ||  final.empty()){
                    final+=result;
                }else{
                    final+="+"+result;
                }
            }
        }
        return simplify(final);

    }

   


    
};




