class Point:
    def __init__(self, x, y, z):
        self.x = float(x)
        self.y = float(y)
        self.z = float(z)

    def print(self):
        return str(self.x) + " " + str(self.y) + " " + str(self.z) + "\n"

    def set_x(self, x):
        self.x = x

    def set_y(self, x):
        self.y = x

    def set_z(self, x):
        self.z = x

class VertricesList:
    def __init__(self):
        self.list = []

    def add(self, x):
        self.list.append(x)

    def print(self):
        out = ""
        for x in self.list:
            out += x.print()
            out += "\n"
        print(out)

    def box_size(self):
        bottom_left = Point(self.min_x(), self.min_y(), self.min_z())
        top_right = Point(self.max_x(), self.max_y(), self.max_z())
        return Box(top_right, bottom_left)

    def min_x(self):
        foo = 1000000
        for x in self.list:
            if x.x < foo:
                foo = x.x
        return foo

    def min_y(self):
        foo = 1000000
        for x in self.list:
            if x.y < foo:
                foo = x.y
        return foo

    def min_z(self):
        foo = 1000000
        for x in self.list:
            if x.z < foo:
                foo = x.z
        return foo

    def max_x(self):
        foo = -1000000
        for x in self.list:
            if x.x > foo:
                foo = x.x
        return foo

    def max_y(self):
        foo = -1000000
        for x in self.list:
            if x.y > foo:
                foo = x.y
        return foo

    def max_z(self):
        foo = -1000000
        for x in self.list:
            if x.z > foo:
                foo = x.z
        return foo

class Box:
    def __init__(self, top_right, bottom_left):
        self.top_right = top_right
        self.bottom_left = bottom_left

    def print(self):
        return self.top_right.print() + self.bottom_left.print()

class Triangle:
    def __init__(self, p1, p2, p3):
        self.p1 = p1
        self.p2 = p2
        self.p3 = p3

filename= "test.txt"

f = open(filename, "r")

numberTriangles = 0
vertricesList = VertricesList()

#Read the points
numberVertrices = f.readline()
# Make list of all vertrices
for x in f:
    x = x.rstrip("\n")
    y = x.split(" ")

    if len(y) == 1:
        numberTriangles = y
        break
    
    vertricesList.add(Point(y[0], y[1], y[2]))

box = vertricesList.box_size()

#Create the boxes 8^2
n = 2
test_l = Point(10,10,10)
test_r = Point(0,0,0)

#calculate bottom right corner for all boxes
#make list of 8^n points
size = pow(8, n)
bottom_right = []
for x in range(0, size):
    point = Point(test_r.x, test_r.y, test_r.z)
    bottom_right.append(point)

#SET X
x_range = test_l.x - test_r.x
step = pow(2, n)
steps_x = x_range / step
for x in range(0, step):
    for y in range(0, size):
        if (size*x/step <= y < size*(x+1)/step):
            bottom_right[y].set_x(x_range*x/step)

#SET Y
y_range = test_l.y - test_r.y
steps_y = y_range / step
for x in range(0, step):
    for y in range(0, size):
        if (size*x/step <= y < size*(x+1)/step and n%2 == 0):
            bottom_right[y].set_y(y_range*x/step)


# DEBUG
for x in bottom_right:
    print(x.print())
