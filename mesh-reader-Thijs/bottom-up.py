#!/usr/bin/env python

from datetime import datetime

startTime = datetime.now()
print("Start time: " + str(startTime))
input_file = "bunny"
hierarchy_file = '{}_hierarchy.txt'.format(input_file)
with open('meshes/{}.mesh'.format(input_file), "r") as mesh:
	with open(hierarchy_file, "w") as hierarchy:
		line = mesh.readline()
		print("vertices: " + line)
		nb_vertices = int(line)
		hierarchy.write(str(nb_vertices) + "\n")
		vertices = []
		for v in range(nb_vertices):
			vertex = mesh.readline()
			vertices.append(vertex)
			hierarchy.write(vertex)
	hierarchy.close()
	line = mesh.readline()
	print("triangles: " + line)
	nb_triangles = int(line)
	triangles = []
	for t in range(nb_triangles):
		triangle = mesh.readline()
		triangles.append(triangle)
mesh.close()
triangles.sort(key = lambda triangle_elem: max(float(vertices[int(i)].split()[1]) for i in triangle_elem.split()), reverse = True)
vertices.clear()
with open(hierarchy_file, "a") as hierarchy:
	stack = [[]]
	while len(triangles) > 0:
		triangle = triangles.pop(0)
		hierarchy.write("t " + triangle.rstrip() + "\n")
		stack[0].append(True)
		for d in range(len(stack)):
			if len(stack[d]) == 2:
				hierarchy.write("b\n")
				stack[d].clear()
				if d == len(stack) - 1:
					stack.append([])
				stack[d+1].append(True)
	first_box = True
	for t in range(len(stack)):
		if len(stack[t]) == 1:
			if not first_box:
				hierarchy.write("b\n")
			first_box = False

hierarchy.close()

print("Runtime: " + str(datetime.now() - startTime))
