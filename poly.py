from sympy import *

f=open("example.txt", "r")
if f.mode == 'r':
    contents =f.read()
print("this is python")

n = symbols('n')
contents= contents.replace(' ','')
print(contents)
print(simplify(contents))
