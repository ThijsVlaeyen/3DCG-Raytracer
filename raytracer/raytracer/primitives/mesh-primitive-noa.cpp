#include <fstream>
#include <iostream>
#include <stack>
#include "primitives/primitives.h"
#include <istream>
#include <sstream>
#include "primitives/bounding-box-primitive.h"

using namespace std;
using namespace raytracer;
using namespace math;
using namespace primitives;

Primitive primitives::mesh_noa(const std::string& file)
{
	ifstream input(file);
	stack<Primitive> thestack;
	string sort;
	double x1, y1, z1, x2, y2, z2, x3, y3, z3;

	while (true)
	{
		input >> sort;
		if (sort == "triangle")
		{
			input >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3;
			const Point3D& p1 = Point3D(x1, y1, z1);
			const Point3D& p2 = Point3D(x2, y2, z2);
			const Point3D& p3 = Point3D(x3, y3, z3);
			Primitive t = triangle(p1, p2, p3);
			thestack.push(t);

		}
		else if (sort == "box")
		{
			int x = 0;
			input >> x;
			std::vector<Primitive> triangles;
			for (int i = 0; i < x; i++)
			{
				Primitive torb = thestack.top();
				thestack.pop();
				triangles.push_back(torb);

			}
			Primitive bbox = bounding_box_accelerator(make_union(triangles));
			thestack.push(bbox);

		}
		else if (sort == "stop")
		{
			std::vector<Primitive> lastboxes;
			for (int i = 0; i < thestack.size(); i++)
			{
				Primitive box1 = thestack.top();
				thestack.pop();
				lastboxes.push_back(box1);
			}
			Primitive finalbox = bounding_box_accelerator(make_union(lastboxes));
			thestack.push(finalbox);
			break;

		}

	}
	Primitive resultbox = thestack.top();
	return resultbox;
	
}
