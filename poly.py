from sympy import *

f=open("example.txt", "r")
if f.mode == 'r':
    contents =f.read()

n = symbols('n')
contents= contents.replace(' ','')
print(simplify(contents))
