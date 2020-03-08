
#include <iostream> 
#include <cstring>
#include <vector>

using namespace std;

class Variable{
    public:
        vector<int> potencia;
        int coef;
        string variable;
        int counterPotencia;

    Variable(int poten, int coef, string variable){
        this->variable = variable;
        this->coef = coef;
        potencia.push_back(poten);
        counterPotencia = 0;
    }
};

class Polynome{
    public:
    vector<Variable> variables;
    string var;
    string coef;

    void add(string variable, int value, int potencia){
        Variable v = Variable(potencia, value, variable);
        v.counterPotencia++;
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
        string coefAcum = "";
        string varAcum = "";


        for(int i=0; i<polinome.length(); i++)
        {                 

            int pot = 0;
            if(polinome[i] == '*'){
                insideP=true;
            }

            //si es un -
            if(polinome[i]== '-' || polinome[i]== '+'){

                if(coefAcum.empty()){
                    coefAcum="1";
                }
                if(var.empty()){
                    varAcum = "const";
                }else if(potenciaT.empty()){
                    pot=1;
                }else{
                    pot=stoi(potenciaT);
                }

                if(find(varAcum) !=-1){
                    variables.at(find(varAcum)).coef+=stoi(coef);
                }else{
                    add(varAcum, stoi(coef), pot);
                }
                varAcum="";
                coefAcum="";
                potenciaT="";
                pot=0;
                insideP=false;

            }  

            else if(isdigit(polinome[i]) && !insideP)
            {

                coefAcum+=polinome[i];
            }
            else if(insideP)
            {
                if(polinome[i] != '*'){
                    potenciaT+=polinome[i];
                    pot = stoi(potenciaT);
                }
            }else{
                varAcum+=polinome[i];
            }

            if(i== polinome.length()-1){
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
                    int a= variables.at(find(varAcum)).coef;
                    int b= stoi(coef);

                    a+=b;
                    variables.at(find(varAcum)).coef = a;
              
                }else
                {
                    add(varAcum, stoi(coefAcum), pot);
                }
                var="";
                coef="";
                potenciaT="0";
                pot=0;
                insideP=false;
            }
            
        }
        
    }

    void multiply(Polynome A, Polynome B)
    {
            
        string varTemp;
        int potenciaTemp;
        int coefTemp;

        for (int i = 0; i < A.variables.size(); i++)
        {
           for(int j=0; j<B.variables.size(); j++)
           {
               string a = A.variables.at(i).variable;
               string b= B.variables.at(j).variable;
              
               if(a.find(b.c_str()) != -1)
               {
                   int position =a.find(b.c_str());
                    potenciaTemp = A.variables.at(i).potencia.at(position)+B.variables.at(j).potencia;
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
    
        Polynome D;
        cout<<"aqui sin simplificar"<<endl;
        cout<<C.printPoly()<<endl;
        D.simplify(C.printPoly());
        cout<<"aqui simplificado"<<endl;
        cout<<D.printPoly()<<endl;

    }


};

int main(){
    Polynome final;
    final.multiplyS("n*2+2", "2n+3"); //2n

}



