from math import log2, ceil

class Triangle:

    def __init__(self, x1, y1, z1, x2, y2, z2, x3, y3, z3):
        self.x1 = x1
        self.y1 = y1
        self.z1 = z1
        self.x2 = x2
        self.y2 = y2
        self.z2 = z2
        self.x3 = x3
        self.y3 = y3
        self.z3 = z3

    def print(self):
        return "triangle " + self.x1 + " " + self.y1 + " " + self.z1 + " " + self.x2 + " " + self.y2 + " " + self.z2 + " " + self.x3 + " " + self.y3 + " " + self.z3

    def min_x(self):
        return min(self.x1, self.x2, self.x3)

    def min_y(self):
        return min(self.y1, self.y2, self.y3)

    def min_z(self):
        return min(self.z1, self.z2, self.z3)

def weight(elem):
    return ((float(elem.x1) + float(elem.x2) + float(elem.x3))/3) + ((float(elem.y1) + float(elem.y2) + float(elem.y3))/3) + ((float(elem.z1) + float(elem.z2) + float(elem.z3))/3)

filename= "bunny.mesh"

f = open(filename, "r")

# Amout of layers (boxes)
# n = 2

numberVertrices = f.readline()
numberTriangles = 0
numberBoxes = 0
listVertrices = []
listTriangles = []
listSortedTriangles = []

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

    coords = ""

    for a in range(3):
        for z in listVertrices[int(y[a])]:
            coords += z
            coords += " "

    z = coords.split(" ")
    triangle = Triangle(z[0], z[1] ,z[2] ,z[3] ,z[4] , z[5], z[6], z[7], z[8])
    listTriangles.append(triangle)


listTriangles.sort(key = weight, reverse = True)

for x in listTriangles:
    listSortedTriangles.append(x.print())

n = ceil(log2(int(numberTriangles[0])))
for i in range(1, n+1):

    temp = []
    triangles = 0

    for x in listSortedTriangles:

        temp.append(x)

        if x[0] == 't':
            triangles += 1

        if triangles == pow(2, i):
            numberBoxes += 1
            temp.append('box 2')
            triangles = 0

    if triangles > pow(2, i)/2:
        numberBoxes += 1
        temp.append('box 2')

    if triangles <= pow(2, i)/2:
        numberBoxes += 1
        temp.append('box 1')

    listSortedTriangles = temp

    temp = []

#listTriangles.append('box 2')
listSortedTriangles.append('stop')

g = open(filename.rstrip(".mesh")+".txt", "w")

for x in listSortedTriangles:
    g.write(x)
    g.write("\n")

print("Done!\nVertrices: " + numberVertrices + "Triangles: " + numberTriangles[0] + "\nBoxes: " + str(numberBoxes))