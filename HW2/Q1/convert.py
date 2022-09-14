# Using readlines()
# file = open('a1.txt', 'r')
file = open('Yeast/167.txt_graph', 'r')

newF = open('formatted.txt', 'w')
Lines = file.readlines()
# print(Lines[:5])  
index = 0
graph_id = 0
n = len(Lines)
while index<n:
    line = Lines[index].strip()
    index+=1
    if len(line) == 0:
        break
    toWrite = 't # ' + str(graph_id) + '\n'
    graph_id+=1
    newF.write(toWrite)
    line = Lines[index].strip()
    index+=1
    nodes = int(line)
    for i in range(nodes):
        line = Lines[index].strip()
        index+=1
        toWrite = 'v ' + str(i) + ' ' + line + '\n'
        newF.write(toWrite)
    line = Lines[index].strip()
    index+=1
    edges = int(line)
    for i in range(edges):
        line = Lines[index].strip()
        index+=1
        toWrite = 'e ' + line + '\n'
        newF.write(toWrite)
    
    index+=1            # TODO check this, there is an empty line after each graph
    print(graph_id,index)

file.close()
newF.close()
    
