from sympy import *

f=open("example.txt", "r")
if f.mode == 'r':
    contents =f.read()

a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z= symbols('{a b c d e f g h i j k l m n o p q r s t u v w x y z}')
contents= contents.replace(' ','')
print("POLINOMIO")
print("T(n)")
print(contents)
print(simplify(contents))
print("COMPLEJIDAD")
result= str(simplify(contents))
result = result[:result.find("+")]
other = ""
if(result.find("**2") != -1):
    other+="n**2";
elif(result.find("n") != -1):
    other+="n";
elif(result.find("**3") != -1):
    other+="n**3";
elif(result.find("**4") != -1):
    other+="n**4";

if(result.find("log") != -1):
    other+="log(n)";
print("O("+other+")")

