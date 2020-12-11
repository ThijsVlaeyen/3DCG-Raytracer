import sys

sys.setrecursionlimit(2000)
# Point class with x y and z coord
class Point:
    def __init__(self, x, y, z):
        self.x = float(x)
        self.y = float(y)
        self.z = float(z)

    def __str__(self):
        return '{} {} {}'.format(self.x, self.y, self.z)

# Triangle class with the 3 points and functions to return max and min of x y z
class Triangle:
    def __init__(self, point1, point2, point3):
        self.p1 = point1
        self.p2 = point2
        self.p3 = point3

    def __str__(self):
        return 'triangle {} {} {}'.format(self.p1, self.p2, self.p3)

    def min_x(self):
        return min(self.p1.x,self.p2.x,self.p3.x)

    def max_x(self):
        return max(self.p1.x,self.p2.x,self.p3.x)

    def min_y(self):
        return min(self.p1.y,self.p2.y,self.p3.y)
    
    def max_y(self):
        return max(self.p1.y,self.p2.y,self.p3.y)

    def min_z(self):
        return min(self.p1.z,self.p2.z,self.p3.z)
    
    def max_z(self):
        return max(self.p1.z,self.p2.z,self.p3.z)

# Functions to check if the triangle is in a box
def x_in_box(triangle, min, max):
    return (triangle.min_x() > min) and (triangle.max_x() < max)

def y_in_box(triangle, min, max):
    return (triangle.min_y() > min) and (triangle.max_y() < max)

def z_in_box(triangle, min, max):
    return (triangle.min_z() > min) and (triangle.max_z() < max)

def lowest_x(trianglesp):
    res = trianglesp[0].min_x()
    for t in trianglesp:
        if t.min_x() < res:
            res = t.min_x()
    return res

def highest_x(trianglesp):
    res = trianglesp[0].max_x()
    for t in trianglesp:
        if t.max_x() > res:
            res = t.max_x()
    return res

def lowest_y(trianglesp):
    res = trianglesp[0].min_y()
    for t in trianglesp:
        if t.min_y() < res:
            res = t.min_y()
    return res

def highest_y(trianglesp):
    res = trianglesp[0].max_y()
    for t in trianglesp:
        if t.max_y() > res:
            res = t.max_y()
    return res

def lowest_z(trianglesp):
    res = trianglesp[0].min_z()
    for t in trianglesp:
        if t.min_z() < res:
            res = t.min_z()
    return res

def highest_z(trianglesp):
    res = trianglesp[0].max_z()
    for t in trianglesp:
        if t.max_z() > res:
            res = t.max_z()
    return res


mesh = open("bunny.mesh", "r")
result = "results_mesh.txt"

# Getting the number of points
nb_points = int(float(mesh.readline()))
print('There are {} points to be created'.format(nb_points))

# Reading all points
points = []
for i in range (0, nb_points):
    next_line = mesh.readline()
    clean_line = next_line.strip('\n').strip('\r')
    coords = clean_line.split(" ")
    point = Point(coords[0], coords[1], coords[2])
    points.append(point)
    print('added {} to the list'.format(point))

# Getting the number of triangles
nb_triangles = int(float(mesh.readline()))
print('There are {} triangles to be created'.format(nb_triangles))

# Reading all triangles
triangles = []
for i in range (0, nb_triangles):
    next_line = mesh.readline()
    clean_line = next_line.strip('\n').strip('\r')
    triangle_points = clean_line.split(" ")
    triangle = Triangle(points[int(triangle_points[0])], points[int(triangle_points[1])], points[int(triangle_points[2])])
    triangles.append(triangle)
    print('added {} to the list'.format(triangle))

def write_box(trianglesp):
    writer = open(result, "a")
    for t in trianglesp:
        writer.write(str(t) + '\n')
    print('writing this to writer {}'.format(str(len(trianglesp))))
    writer.write("box " + str(len(trianglesp)) + "\n")
    writer.close()

def create_box_structure(trianglesp):
    if len(trianglesp) > 2:
        x_width = highest_x(trianglesp) - lowest_x(trianglesp)
        y_width = highest_y(trianglesp) - lowest_y(trianglesp)
        z_width = highest_z(trianglesp) - lowest_z(trianglesp)

        part1 = []
        part2 = []

        # get the widest dimension and use that
        if max(x_width, y_width, z_width) == x_width:
            min_x = lowest_x(trianglesp)
            mid_x = min_x + (x_width / 2)
            for t in trianglesp:
                part1.append(t) if x_in_box(t, min_x, mid_x) else part2.append(t)
        elif max(x_width, y_width, z_width) == y_width:
            min_y = lowest_y(trianglesp)
            mid_y = min_y + (y_width / 2)
            for t in trianglesp:
                part1.append(t) if y_in_box(t, min_y, mid_y) else part2.append(t)
        elif max(x_width, y_width, z_width) == z_width:
            min_z = lowest_z(trianglesp)
            mid_z = min_z + (z_width / 2)
            for t in trianglesp:
                part1.append(t) if z_in_box(t, min_z, mid_z) else part2.append(t)
        
        if len(part1) == 0:
            for i in range(len(part2) // 2):
                part1.append(part2[0])
                part2.remove(part2[0])
        
        create_box_structure(part1)
        create_box_structure(part2)

        boxes = open(result, "a")
        boxes.write("box 2\n")
        boxes.close()

    elif len(trianglesp) > 0:
        write_box(trianglesp)
    else:
        return

# Cleanup of file if it exists
clear_file = open(result, "w")
clear_file.write("")
clear_file.close()

# Write boxes to file
print('Bounding boxes are being made!')
create_box_structure(triangles)

# Exit
print("Processing done, closing file")
close = open(result, "a")
close.write("stop\n")
close.close()
mesh.close()
print("Finished")