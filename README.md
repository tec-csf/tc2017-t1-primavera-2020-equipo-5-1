# Tarea 1. Complejidad computacional

---

##### Integrantes:
1. Carla Perez Gavilan Del Castillo - A01023033 - CSF
2. Christain Dalma Schultz - A01423166- CSF
3. Ruben Hernandez Rivas - A01024669 - CSF

---
## 1. Aspectos generales

Las orientaciones de la tarea se encuentran disponibles en la plataforma **Canvas**.

Este documento es una guía sobre qué información debe entregar como parte de la tarea, qué requerimientos técnicos debe cumplir y la estructura que debe seguir para organizar su entrega.


### 1.1 Requerimientos técnicos

A continuación se mencionan los requerimientos técnicos mínimos de la tarea, favor de tenerlos presente para que cumpla con todos.

* El código debe desarrollarse en C++, cumpliendo con el último estándar [C++17](https://isocpp.org/std/the-standard).
* Toda la programación debe realizarse utilizando Programación Genérica.
* Deben utilizarse las [C++ Core Guidelines](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md).
* Todo el código debe estar correctamente documentado, siguiendo los lineamientos que aparecen en [Documenting C++ Code](https://developer.lsst.io/cpp/api-docs.html).
* Todo el código de la tarea debe alojarse en este repositorio de GitHub.
* Debe configurar su repositorio para que utilice el sistema de Integración Continua [Travis CI](https://travis-ci.org/).

### 1.2 Estructura del repositorio

El proyecto debe seguir la siguiente estructura de carpetas:
```
- / 			        # Raíz del repositorio
    - README.md			# Archivo con la información general de la actividad (este archivo)
    - sources  			# Códigos fuente con la solución
    - examples			# Archivos de ejemplo que pueden utilizarse para verificar que la solución funciona.
```

## 2. Solución

El programa se basa en el siguiente algoritmo:
 # 0. LECTURA DEL ARCHIVO: 
 Se lee el archivo y se almacena en un vector de Instrucciones. 
 # 1. IDENTIFICAR INSTRUCCIÓN: 
 Identificar qué instrucción es a través de una búsqueda dentro de la palabra: cout, printf, return, for, while, if, else. Cada instrucción te llevará a su método correspondiente. 
 # 2 CALCULO DE COMPLEJIDAD DE ALGUNAS INSTRUCCIÓNES
        # a. FOR: Para el caso del for se hace una división por incremento, inicialización y comparación. Se multiplica el incremento y la comparación por la cantidad de veces que se repite el ciclo. 
        # b. WHILE : Para el caso del while se hace una iteración en todo lo que está entre llaves para encontrar el incremento. Después se sigue un método similar al for. 
 # 3 OPERACIONES ELEMENTALES:
En caso de que no se encuentra ninguna de las palabras claves se utiliza el método de operaciones elementales, que nos permite determinar cuántas operaciones elementales hay en una línea determinada a través del uso de signos. 
 # 4 CICLOS: 
 Los cíclos delimitados por las llaves al inicio y al final, se calculan cuando se encuentra la palabra for o while después de las cuales se ingresa a una pila. Siempre y cuando la pila está llena se multiplicará a todas las líneas por todos los ciclos. 
 # 5 MULTIPLICACIÓN Y CÁLCULO FINAL:
 Se multiplican los ciclos por la cantidad de operaciones elementales y la complejidad de cada una de las líneas del código. Se acumulan los términos 
 # 6 SIMPLIFICACIÓN DEL POLINOMIO FINAL Y COTA ASINTÓTICA
 Se simplifa el polinomio final utlizando la librera de python de sympy. La cadena de caracteres se pasa por medio de un archivo txt. La cota asintótica se determina como el primer término de la izquierda a derecha una vez se han acomodado los términos simplificados del menor al mayor. 

### 2.1 Pasos a seguir para utilizar la aplicación

1. Crear un archivo cpp con el código que quiere analizar, asegurarse que este archivo esté dentro de la carpeta del programa. 
#### RESTRICCIONES: las variables que se incluyan en el archivo deben de tener un nombre de una letra mayúscula o minúscula del abecedario, de lo contrario se calculará su complejidad pero no se podrá ver la versión simplificada. 
2. En la terminar correr los siguientes comandos de compilación con c++17: 
 - g++ analyzer.cpp -std=c++17 -o travis_gcc_cpp17
 -  ./travis_gcc_cpp17 -i code.cpp
 -  python poly.py
 3. Es importante tener descargado con antelación en compilador g++ así como la librería de simpy. En caso de no tenerlos, correr los siguientes comandos:
   - pip install sympy


## 3. Referencias

 
