
#include <iostream> 
#include <cstring>
#include <vector>

using namespace std;

class Variable{
    public:
        int potencia;
        int coef;
        string variable;

    Variable(int potencia, int coef, string variable){
        this->variable = variable;
        this->coef = coef;
        this->potencia = potencia;
    }
};

class Polynome{
    public:
    vector<Variable> variables;
    string var;
    string coef;

    void add(string variable, int value, int potencia){
        Variable v = Variable(potencia, value, variable);
        variables.push_back(v);
       
    }


    Polynome(){}

    int find(string var){
         for (int i = 0; i < variables.size(); i++)
        {
            string a = variables.at(i).variable;
            if(strcmp(a.c_str(), var.c_str())== 0){
                return i;
            }
        }
        return -1;
    }

    void simplify(string polinome)
    {
        bool insideP= false;
        string potenciaT = "";
        for(int i=0; i<polinome.length(); i++)
        {                 

            int pot=0;
            int p= 0;
            if(polinome[i] == '*'){
                insideP=true;
            }

            //si es un +
            if(polinome[i]== '-'){

                if(coef.empty()){
                    coef="1";
                    pot = 0;
                }
                if(var.empty()){
                    var = "const";
                    pot=0;
                }else if(potenciaT.empty()){
                    pot=1;
                }else{
                    pot=stoi(potenciaT);
                }

                if(find(var) !=-1){
                    variables.at(find(var)).coef+=stoi(coef);
                }else{
                    add(var, stoi(coef), pot);
                }
                var="";
                coef="";
                potenciaT="";
                pot=0;
            } 
            
            //si es un -
            else if(polinome[i]== '+'){

                 if(coef.empty()){
                    coef="1";
                    pot=1;
                }
                if(var.empty()){
                    var = "const";
                    pot = 0;
                }else if(potenciaT.empty()){
                    pot=1;
                }else{
                    pot=stoi(potenciaT);
                }
                
                if(find(var) != -1){

                    variables.at(find(var)).coef+=stoi(coef);

                  
                }else{
                    add(var, stoi(coef), pot);

                }
                var="";
                coef="";
                potenciaT="";
                pot=0;
                insideP=false;

           
            }
            
            //si es un número 
            else if(isdigit(polinome[i]) && !insideP)
            {

                coef+=polinome[i];
            }

            //si es potencia
            else if(insideP)
            {
                if(polinome[i] != '*'){
                    potenciaT+=polinome[i];
                    p = stoi(potenciaT);
                }
                

            }else{

                var+=polinome[i];
            }

            if(i== polinome.length()-1){
               cout<<" this is the var for "<<var;
                if(var.empty()){
                    var = "const";
                    pot= 0;
                }else if(potenciaT.empty()){
                    pot=1;
                }else{
                    pot=stoi(potenciaT);
                }
                if(find(var) != -1)
                { 
                    int a= variables.at(find(var)).coef;
                    int b= stoi(coef);

                    a+=b;
                    variables.at(find(var)).coef = a;
                    insideP=false;
              

                }else
                {
                    add(var, stoi(coef), pot);
                    insideP=false;
                   
                }
                var="";
                coef="";
                potenciaT="0";
                pot=0;
            }
            
        }
        
    }

    void multiply(Polynome A, Polynome B)
    {
            
        int count = 0;
        string varTemp;
        int potenciaTemp;
        int coefTemp;
        for (int i = 0; i < A.variables.size(); i++)
        {
           for(int j=0; j<B.variables.size(); j++)
           {
               string a = A.variables.at(i).variable;
               string b= B.variables.at(j).variable;
              
               if(strcmp( a.c_str(), b.c_str()) == 0)
               {
                    potenciaTemp = A.variables.at(i).potencia+B.variables.at(j).potencia;
                    varTemp= a;
               }else
               {
                   string con ="const";
                   if(strcmp(a.c_str(), con.c_str()) == 0)
                   {
                    a="";
                    potenciaTemp = B.variables.at(i).potencia ;

                   }
                   if(strcmp(b.c_str(), con.c_str()) == 0)
                   {
                    b="";
                    potenciaTemp = A.variables.at(i).potencia ;

                   }
                   varTemp=a+b;
               }
                int coefa = A.variables.at(i).coef;
                int coefb = B.variables.at(j).coef;

               coefTemp=A.variables.at(i).coef*B.variables.at(j).coef;
               if(varTemp.empty()){
                   varTemp="const";
                   potenciaTemp = 0;
               }
               cout<<coefa<<a<<"^"<<A.variables.at(i).potencia<<" times "<<coefb<<b<<"^"<<B.variables.at(i).potencia<<endl;
               cout<<" equals "<<coefTemp<<varTemp<<" to the power of"<<potenciaTemp<<endl;
                add(varTemp, coefTemp, potenciaTemp);

              
           }
        }
    }

    string printPoly(){
    string final;
    for (int i = 0; i <variables.size(); i++)
        {
            string printPot;
            string printVar;
            string symbol;
            if(variables.at(i).potencia == 0 || variables.at(i).potencia == 1){
                printPot ="";
            }else{
                int pot = variables.at(i).potencia;
                printPot="*"+ to_string(pot);
            }

            if(i==0){
                symbol="";
            }else if(variables.at(i).coef>0){
                symbol="+";
            }else{
                symbol="";
            }

            final+=symbol+to_string(variables.at(i).coef)+variables.at(i).variable+printPot;

        }
        return final;
    }

    void multiplyS(string a, string b){
        Polynome A;
        A.simplify(a);
        

        Polynome B;
        B.simplify(b);


        Polynome C;
        C.multiply(A,B);
    
        cout<<C.printPoly()<<endl;

    }


};




