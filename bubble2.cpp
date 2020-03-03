#include <iostream>
#include <cstdlib>
#include <time.h>
#include<stdlib.h>
#include <chrono>

using namespace std;
using namespace std::chrono;


template <class T>
class Ordenar{
    public:
        void virtual sort(T* arreglo, int size)=0;
        void intercambiar(T* arreglo, int a, int b){
            T temp=arreglo[a];
            arreglo[a]= arreglo[b];
            arreglo[b]=temp;
        }

        void imprimir(T *arreglo, int size){
            for(int i=0; i<size; i++){
                cout<<arreglo[i]<<",";
            }
            cout<<endl;
        }
};

template <class T>
class BrickSort: public Ordenar<T>{
    public:
        void sort(T *arreglo, int size){
            for(int i=size; i>=0; i--){
                for(int j=size; j>0; j--){
                    if(arreglo[j]<arreglo[j-1]){
                       this->intercambiar(arreglo, j, j-1);
                    }
                }
            }
        }
};

template <class T>
class BubbleSort: public Ordenar<T>{
    public:
        void sort(T *arreglo, int size){
            for(int i=0; i<size; i++){
                for(int j=0; j<size-1; j++){
                    if(arreglo[j]>arreglo[j+1]){
                            this->intercambiar(arreglo, arreglo[j], arreglo[j+1]);
                    }
                }
            }
        }
};

template <class T>
class ShakeSort: public Ordenar<T>{
    public:
        void sort(T *arreglo, int size){
           for(int i= 0; i<size;){
               for(int j = i+1; j < size; i++)
		        {
			        if(arreglo[j] < arreglo[j-1])
				    this->intercambiar(arreglo, j, j+1);
		        }
                size--;
                for(int j = size-1; j > i; j--)
		        {
			            if(arreglo[j] < arreglo[j-1])
				        this->intercambiar(arreglo, j, j-1);
		        }
		        i++;
           }

        }
};




int main(){

    //  Crea un arreglo de 10,000 números aleatorios de 0 a 99,999.
    int size=10;
    int arreglo[size];
    for(int i=0; i<size; i++){
        arreglo[i]= rand()%100000;
    }


    // 2. Mide e imprime el tiempo que tarda el arreglo en ser ordenado utilizando InsertionSort
    cout<<"BRICK  SORT"<<endl;
    auto inicio = high_resolution_clock::now();

    BrickSort<int> b;
    b.sort(arreglo, size); 
    
     for(int i=0; i<size; i++){
        cout<<arreglo[i]<<endl;
    }

    auto fin = high_resolution_clock::now();

    cout<<"tiempo de ordenamiento :"<<duration_cast<nanoseconds>(fin-inicio).count()<<endl;


    // 3. Asigna nuevos números aleatorios al arreglo (ya que quedo ordenado)
    srand(time(NULL));
    for(int i=0; i<size; i++){
        arreglo[i]= rand()%100;
    }

    cout<<"SHAKE  SORT"<<endl;


    ShakeSort<int> s;
    s.sort(arreglo, size); 
    
     for(int i=0; i<size; i++){
        cout<<arreglo[i]<<endl;
    }

    fin = high_resolution_clock::now();

    cout<<"tiempo de ordenamiento :"<<duration_cast<nanoseconds>(fin-inicio).count()<<endl;


}