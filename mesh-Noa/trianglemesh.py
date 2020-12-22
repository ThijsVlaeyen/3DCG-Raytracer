import sys

class Coordinaat:
	"""docstring for Coordinaat"""
	def __init__(obj, x, y, z):
		obj.x = x
		obj.y = y
		obj.z = z

	def __str__(self):
		return "{} {} {}".format(self.x, self.y, self.z)

class Triangle:
	"""docstring for Triangle"""
	def __init__(obj, c1, c2, c3):
		obj.c1 = c1
		obj.c2 = c2
		obj.c3 = c3

	def __str__(self):
		return "triangle {} {} {}".format(self.c1, self.c2, self.c3)

	# Max en Min bepalen in iedere richting
	def calc_max_x(self):
		return max(self.c1.x, self.c2.x, self.c3.x)

	def calc_min_x(self):
		return min(self.c1.x, self.c2.x, self.c3.x)

	def calc_max_y(self):
		return max(self.c1.y, self.c2.y, self.c3.y)

	def calc_min_y(self):
		return min(self.c1.y, self.c2.y, self.c3.y)

	def calc_max_z(self):
		return max(self.c1.z, self.c2.z, self.c3.z)

	def calc_min_z(self):
		return min(self.c1.z, self.c2.z, self.c3.z)


