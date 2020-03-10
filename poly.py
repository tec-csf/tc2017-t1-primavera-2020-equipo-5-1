from sympy import *

f=open("example.txt", "r")
if f.mode == 'r':
    contents =f.read()

a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z= symbols('{a b c d e f g h i j k l m n o p q r s t u v w x y z}')
contents= contents.replace(' ','')
print("T(n)")
print(contents)
print(simplify(contents))
print("Cota Asintótica")
result= str(simplify(contents))
print(result[:result.find("+")])

