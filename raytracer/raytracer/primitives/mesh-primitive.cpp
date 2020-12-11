#include "primitives/primitives.h"
#include "primitives/bounding-box-primitive.h"
#include "primitives/mesh-primitive.h"
#include "easylogging++.h"
#include <fstream>
#include <iostream>
#include <stack>
#include <istream>
#include <sstream>

using namespace raytracer;
using namespace primitives;
using namespace math;
using namespace std;

namespace raytracer
{
	Primitive primitives::mesh(const std::string& filename)
	{
		ifstream input(filename);
		stack<Primitive> stack;
		string sort;
		double x1, x2, x3, y1, y2, y3, z1, z2, z3;

		while (true)
		{
			input >> sort;
			//check if it is a triangle, box or the end of the file
			if (sort == "triangle")
			{
				//read all points of the line
				input >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3;

				//make the points themselves
				const Point3D& p1 = Point3D(x1, y1, z1);
				const Point3D& p2 = Point3D(x2, y2, z2);
				const Point3D& p3 = Point3D(x3, y3, z3);

				//make a triangle
				Primitive t = triangle(p1, p2, p3);
				//push the triangle to the stack
				stack.push(t);
			}
			else if (sort == "box")
			{
				//check how many we need to put in a box
				int x = 0;
				input >> x;
				//list of all triangle and/or boxes we want to put in the box
				std::vector<Primitive> trianglelist;
				for (int i = 0; i < x; i++)
				{
					Primitive p = stack.top();
					//delete primitive from the stack
					stack.pop();
					//add it to the list
					trianglelist.push_back(p);
				}
				Primitive box = bounding_box_accelerator(make_union(trianglelist));
				stack.push(box);
			}
			else if (sort == "stop")
			{
				std::vector<Primitive> boxes;
				for (int i = 0; i < stack.size(); i++)
				{
					Primitive box = stack.top();
					stack.pop();
					boxes.push_back(box);
				}
				Primitive mainBox = bounding_box_accelerator(make_union(boxes));
				stack.push(mainBox);
				break;
			}
			else
			{
				cout << "Something went wrong  with the type of primitive" <<
					sort << endl;
			}
		}
		Primitive boundingBox = stack.top();
		return boundingBox;
	}
}

