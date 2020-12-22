import sys
from trianglemesh import Coordinaat
from trianglemesh import Triangle

sys.setrecursionlimit(2000)

# Functions to check if the triangle is in a box
def x_in_box(triangle, min, max):
    return (triangle.calc_min_x() > min) and (triangle.calc_max_x() < max)

def y_in_box(triangle, min, max):
    return (triangle.calc_min_y() > min) and (triangle.calc_max_y() < max)

def z_in_box(triangle, min, max):
    return (triangle.calc_min_z() > min) and (triangle.calc_max_z() < max)

def lowest_x(trianglesp):
    res = trianglesp[0].calc_min_x()
    for t in trianglesp:
        if t.calc_min_x() < res:
            res = t.calc_min_x()
    return res

def highest_x(trianglesp):
    res = trianglesp[0].calc_max_x()
    for t in trianglesp:
        if t.calc_max_x() > res:
            res = t.calc_max_x()
    return res

def lowest_y(trianglesp):
    res = trianglesp[0].calc_min_y()
    for t in trianglesp:
        if t.calc_min_y() < res:
            res = t.calc_min_y()
    return res

def highest_y(trianglesp):
    res = trianglesp[0].calc_max_y()
    for t in trianglesp:
        if t.calc_max_y() > res:
            res = t.calc_max_y()
    return res

def lowest_z(trianglesp):
    res = trianglesp[0].calc_min_z()
    for t in trianglesp:
        if t.calc_min_z() < res:
            res = t.calc_min_z()
    return res

def highest_z(trianglesp):
    res = trianglesp[0].calc_max_z()
    for t in trianglesp:
        if t.calc_max_z() > res:
            res = t.calc_max_z()
    return res

inputfile = open("F:/school5/3d/project/ray-tracer-noah/mesh-Noa/meshes/bunny.mesh", "r")
outputfile = "F:/school5/3d/project/ray-tracer-noah/mesh-Noa/donemesh/bunny.txt"

coordinaten = []
aantal = inputfile.readline()
for x in range(int(aantal)):
    xyz = inputfile.readline()
    clean_line = xyz.strip('\n').strip('\r')
    string = clean_line.split(" ")
    x = float(string[0])
    y = float(string[1])
    z = float(string[2])
    coordinaten.append(Coordinaat(x, y, z))
driehoeken = []

aantal = inputfile.readline()
for x in range(0, int(float(aantal))):
    driehoek = inputfile.readline()
    properelijn = driehoek.strip('\n').strip('\r')
    driehoekpunten = driehoek.split(" ")
    coordinaat1 = int(driehoekpunten[0])
    coordinaat2 = int(driehoekpunten[1])
    coordinaat3 = int(driehoekpunten[2])
    driehoeken.append(Triangle(coordinaten[coordinaat1], coordinaten[coordinaat2], coordinaten[coordinaat3]))

def write_box(trianglesp):
    writer = open(outputfile, "a")
    for t in trianglesp:
        writer.write(str(t) + '\n')
    writer.write("box " + str(len(trianglesp)) + "\n")
    writer.close()

def maak_doos_structuur(trianglesp):
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
        
        maak_doos_structuur(part1)
        maak_doos_structuur(part2)

        dozen = open(outputfile, "a")
        dozen.write("box 2\n")
        dozen.close()

    elif len(trianglesp) > 0:
        write_box(trianglesp)
    else:
        return

clear_file = open(outputfile, "w")
clear_file.write("")
clear_file.close()

maak_doos_structuur(driehoeken)
close = open(outputfile, "a")
close.write("stop\n")
close.close()
