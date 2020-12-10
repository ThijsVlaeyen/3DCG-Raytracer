meshname = "bunny"
outputfile = 'donemesh/{}.txt'.format(meshname)
inputfile = 'meshes/{}.mesh'.format(meshname)




with open(inputfile, "r") as mesh:
    with open(outputfile, "w") as file2:

        line = mesh.readline()
        nb_vertices = int(line)
        file2.write(str(nb_vertices) + "\n")

        vertices = []
        for v in range(nb_vertices):
            vertex = mesh.readline()
            vertices.append(vertex)
            file2.write(vertex)



    line = mesh.readline()
    
    nb_triangles = int(line)
    triangles = []
    for t in range(nb_triangles):
        triangle = mesh.readline()
        triangles.append(triangle)


triangles.sort(key = lambda triangle_elem: max(float(vertices[int(i)].split()[1]) for i in triangle_elem.split()), reverse = True)
vertices.clear()

class Box:
    def __init__(*children):
        self.children = children

def optimize(triangles):
    if len(triangles) < 10:
        return Box(*triangles)

    (left, right) = split_triangles(triangles)

    oleft = optimize(left)
    oright = optimize(right)

    return Box(oleft, oright)


def write(root):
    if isinstance(root, Triangle):
        print(f"t ....")
    else:
        for child in box.children:
            write(child)
        ...

with open(outputfile, "a") as file2:

    stack = [[]]
    print(len(triangles))
    while len(triangles) > 0:
        triangle = triangles.pop(0)
        file2.write("t " + triangle.rstrip() + "\n")
        stack[0].append(True)

        for d in range(len(stack)):
            if len(stack[d]) == 2:
                file2.write("b\n")
                stack[d].clear()
                if d == len(stack) - 1:
                    stack.append([])
                stack[d+1].append(True)
    first_box = True

    for t in range(len(stack)):
        if len(stack[t]) == 1:
            if not first_box:
                file2.write("b\n")
            first_box = False
file2.close()
