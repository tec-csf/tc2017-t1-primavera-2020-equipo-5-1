from sympy import *

f=open("example.txt", "r")
if f.mode == 'r':
    contents =f.read()

a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z= symbols('{a b c d e f g h i j k l m n o p q r s t u v w x y z}')
A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z= symbols('{A B C D E F G H I J K L M N O P Q R S T U V W X Y Z}')
contents= contents.replace(' ','')
print("T(n)")
print(contents)
print(simplify(contents))
print("Cota Asintótica")
result= str(simplify(contents))
print(result[:result.find("+")])

