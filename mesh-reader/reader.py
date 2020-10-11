from math import log2, ceil

filename= "armadillo.mesh"

f = open(filename, "r")

# Amout of layers (boxes)
# n = 4

numberVertrices = f.readline()
numberTriangles = 0
numberBoxes = 0
listVertrices = []
listTriangles = []

# Make list of all vertrices
for x in f:
    x = x.rstrip("\n")
    y = x.split(" ")

    if len(y) == 1:
        numberTriangles = y
        break
    
    listVertrices.append(y)

# Make list of all triangles
for x in f:
    x = x.rstrip("\n")
    y = x.split(" ")

    out = "t "

    for a in range(3):
        for z in listVertrices[int(y[a])]:
            out += z
            out += " "

    listTriangles.append(out)

n = ceil(log2(int(numberTriangles[0])))
for i in range(1, n+1):

    temp = []
    triangles = 0

    for x in listTriangles:

        temp.append(x)

        if x[0] == 't':
            triangles += 1

        if triangles == pow(2, i):
            numberBoxes += 1
            temp.append('b')
            triangles = 0

    listTriangles = temp

    temp = []

listTriangles.append('b')

g = open(filename.rstrip(".mesh")+".txt", "w")

for x in listTriangles:
    g.write(x)
    g.write("\n")

print("Done!\nVertrices: " + numberVertrices + "Triangles: " + numberTriangles[0] + "\nBoxes: " + str(numberBoxes))