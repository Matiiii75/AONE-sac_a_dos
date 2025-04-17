datafileName = 'DKP_Instances/toy_instance.txt'

with open(datafileName, "r") as file:
    line = file.readline()  
    nbItems = int(line)
    
    line = file.readline()  
    capacityMax = int(line)

    profits = []
    weights = []
    
    for i in range(nbItems):
        line = file.readline()
        lineTab = line.split()
        
        profits.append(int(lineTab[1])) 
        weights.append(int(lineTab[2]))

    line = file.readline()  
    lineTab = line.split()

    nbCliques = int(lineTab[0])
    cliques = []

    for i in range(nbCliques):
        line = file.readline()
        lineTab = line.split()

        size = int(lineTab[0])
        cl = []

        for j in range(size):
            cl.append(int(lineTab[j+1]))

        cliques.append(cl)
    
print("Nb Items : ", nbItems)
print("Capacity : ", capacityMax)
print("Nb cliques : ", nbCliques)
print()
print("Profits : ", profits)
print("Weights : ", weights)
print("Cliques : ", cliques)

# For each item, the set of cliques it belongs to
inCliques = [[] for i in range(nbItems)]

for k in range(nbCliques):
    for i in cliques[k]:
        inCliques[i].append(k)

print("inCliques : ", inCliques)
