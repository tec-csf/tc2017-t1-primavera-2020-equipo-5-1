
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
#include <iomanip>


using namespace std;

/**
 * it creates an object that allows the user to save the complexity, cylce, number of line, and elemental operations that each line of code hase
 * 
 * @note this object is later used as part of a vector
 */

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