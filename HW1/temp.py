## CHECK COUNT OF A PARTICULAR ELEMENT

f = open("webdocs.dat", "r")
x1=0
x2=0
while True:
    line = f.readline()
    if not line:
        break
    k = line.split(" ")
    x1+=1
    if ('122' in k) or ('122\n' in k):
        x2+=1 
print(x1,x2,2*x2)