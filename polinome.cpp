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
#include <map>
#include <vector>
#include <sstream>


using namespace std;

/**
* Creation of both the class for Variables and the class for Mono
*/

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
    
    /**
    * Void to turn to monome
    *
    * @param[in] string fo the mono
    */
    void turnToMonome(string mono){
        string acumCoef = "";
        string acumVar = "";
        string acumPot = "";
        bool getPot = false;
        bool logar = false;
        for(int i=0; i<mono.length(); i++){

            if(mono[i+1] == '('){
                acumVar+=mono[i];
                logar = true;
            }else if(mono[i] == ')'){
                acumVar+=mono[i];
                logar = false;
            }
            else if(isdigit(mono[i]) || mono[i]=='-'){
                if(!logar){
                    if(getPot){
                    acumPot+= mono[i];
                    }else{
                    acumCoef+=mono[i];
                    }
                }else{
                    acumVar+=mono[i];
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

                    if(acumCoef =="-"){
                        acumCoef="-1";
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
                    if(acumCoef =="-"){
                        acumCoef="-1";
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
    

    /**
    *
    *Function to print
    *
    *@param[Out] print of the mono with power, variable and coeficent
    */
    
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

//####################################################################################################################

/**
Polynomial fucntion
*/

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
        bool logar = false;
        for(int i=0; i<instruction.length(); i++){
            if(instruction[i+1]=='('){
                logar=true;
            }else if(instruction[i]==')'){
                logar= false;
            }
            if(i== instruction.length()-1){
                if(isdigit(instruction[i]) && !varStart){
                        acumCoef+=instruction[i];
                    }else{
                        if(instruction[i]!= ' ' ){
                            acumVar+=instruction[i];
                        }
                    }
                 if(!logar){
                if(acumCoef.empty()){
                    acumCoef = "1";
                }
                if(acumVar.empty()){
                    acumVar = "const";
                }
                if(acumCoef!="0" && acumCoef!="-0"){
                    if(acumCoef=="-"){
                        acumCoef ="-1";
                    }

                    addPoly(acumVar, stoi(acumCoef));
                    if(instruction[i]=='-'){
                        acumCoef="-";
                    }else{
                        acumCoef="";
                    }
                    varStart = false;
                    acumVar="";
                    }
                 }      

            }else if(instruction[i]=='+' || (instruction[i]=='-' && i!=0)){
                if(!logar){
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
                }else{
                 acumVar+=instruction[i];
                }
                
            }else if((isdigit(instruction[i]) || instruction[i] == '-') && !varStart){
                acumCoef+=instruction[i];
            }else{
                if(instruction[i]!= ' '){
                acumVar+=instruction[i];
                }
                varStart = true;
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

    /**
    *
    * Function for adding the polinomials 
    *
    *@param[in] a string of the variables and the coeficient
    *@param[out] the result of the aded polinomial
    */
    
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
            if(currentVar.coef == 1 && currentVar.variable != "const"){
                printCoef="";
            }else if(currentVar.coef == -1 && currentVar.variable != "const"){
                printCoef="-";
            }
            else{
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
            final+=printCoef+printVar+printPot+".";
        }
        final= final.substr(0, final.length()-1);
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
            if(final.findVar(current.variable) == -1 ){
                final.addVar(current.potencia, 1, current.variable);
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
        if(final.variables.size()>1){
            for(int i=0; i< final.variables.size(); i++){
            if(strcmp(final.variables.at(i).variable.c_str(), "const") == 0 && final.variables.at(i).coef==1){
                final.variables.erase(final.variables.begin()+i);
            }
            }
        }
        
        return mono2str(final);

    }

    string multiply(string a, string b){
        string acumMono;
        Poly pola;
        Poly polb;
        string final ="";
        bool logar = false;
        for(int i=0; i<a.length(); i++){
             if(a[i]=='('){
                logar = true;
                acumMono+=a[i]; 
 
            }else if(a[i]==')'){
                logar = false;
                acumMono+=a[i];  
                pola.monomes.push_back(acumMono);
                acumMono="";

            }
            else if(a[i]=='+' || a[i]=='-'){
                if(acumMono != "0" && acumMono != ""){
                    if(logar == false){
                        pola.monomes.push_back(acumMono);
                    }else{
                        acumMono+=a[i];  


                    }
                }
                if(!logar){
                if(a[i]=='-'){
                        acumMono="-";
                }else{
                    acumMono="";
                }
                }
                
            }else{
                acumMono+=a[i];  
            }
            
            if(i==a.length()-1){
                if(acumMono != "0" && acumMono != ""){
                    if(!logar){
                    pola.monomes.push_back(acumMono);
                    }else{
                        if(a[i]!=')'){
                            acumMono+=a[i];  
                        }
                    }
                }
                if(!logar){
                if(a[i]=='-'){
                        acumMono="-";
                }else{
                    acumMono="";
                }
                }
            }

        }

        for(int i=0; i<b.length(); i++){
             if(b[i]=='('){
                logar = true;
                acumMono+=b[i]; 
 
            }else if(b[i]==')'){
                logar = false;
                acumMono+=b[i];  
                polb.monomes.push_back(acumMono);
                acumMono="";

            }
            else if(b[i]=='+' || b[i]=='-'){
                if(acumMono != "0" && acumMono != ""){
                    if(logar == false){
                        polb.monomes.push_back(acumMono);
                    }else{
                        acumMono+=b[i]; 

                    }
                }
                if(!logar){
                if(b[i]=='-'){
                        acumMono="-";
                }else{
                    acumMono="";
                }
                }
                
            }else{
                acumMono+=b[i];  
            }
            
            if(i==b.length()-1){
                if(acumMono != "0" && acumMono != ""){
                    if(!logar){
                    polb.monomes.push_back(acumMono);
                    }else{
                        if(b[i]!=')'){
                            acumMono+=b[i];  
                        }
                    }
                }
                if(!logar){
                if(b[i]=='-'){
                        acumMono="-";
                }else{
                    acumMono="";
                }
                }
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
                cout<<pola.monomes.at(i)<<" times "<<polb.monomes.at(j)<<" equals "<<result<<endl;
            }
        }
        flush();
        return final;

    }

    
};

/**
Main function of the polinome.cpp
*/

int main(){
    Poly p;
    cout<<p.multiply("2n+3", "m+2")<<endl;
}



