#include "mesh-primitive-thijs.h"
#include "union-primitive.h"
#include "bounding-box-primitive.h"
#include "bounding-box-hierarchy.h"
#include "triangle-primitive.h"
#include "util/misc.h"
#include "performance/performance.h"
#include "easylogging++.h"
#include <assert.h>
#include <fstream>
#include <sstream>

using namespace raytracer;
using namespace raytracer::primitives;
using namespace math;


namespace
{
	class MeshPrimitive : public raytracer::primitives::_private_::PrimitiveImplementation
	{
	public:
		Primitive primitive;

		MeshPrimitive(const std::string p) : primitive(create_hierarchy(p)) {}

		Primitive create_hierarchy(const std::string p) {
			LOG(INFO) << "Started building hierarchy";
			std::ifstream file;
			file.open(p);
			if (!file) {
				LOG(INFO) << "File not found";
			}
			std::string line;
			std::getline(file, line);
			uint64_t nr = std::stoi(line);
			std::vector<Point3D> vertices;
			for (int i = 0; i < nr; i++) {
				std::getline(file, line);
				std::istringstream iss(line);
				double a, b, c;
				if (!(iss >> a >> b >> c)) { break; }
				vertices.push_back(Point3D(a, b, c));
			}
			Primitive primitive = read_hierarchy(file, vertices);
			file.close();
			LOG(INFO) << "Hierarchy made";
			return primitive;
		}

		Primitive read_hierarchy(std::ifstream& file, const std::vector<Point3D>& vertices) {
			LOG(INFO) << "Started reading hierarchy";
			std::string line;
			std::vector<Primitive> primitives;
			while (std::getline(file, line)) {
				if (line == "-1") {
					std::vector<Primitive> combine;
					combine.push_back(primitives.back());
					primitives.pop_back();
					combine.push_back(primitives.back());
					primitives.pop_back();
					primitives.push_back(bounding_box_hierarchy(combine));
				}
				else {
					std::vector<Primitive> triangles;
					int nr = std::stoi(line);
					for (int i = 0; i < nr; i++) {
						std::getline(file, line);
						std::istringstream iss(line);
						int a, b, c;
						if (!(iss >> a >> b >> c)) { break; }
						triangles.push_back(bounding_box_accelerator(triangle(vertices[a], vertices[b], vertices[c])));
					}
					primitives.push_back(bounding_box_hierarchy(triangles));
				}
			}
			LOG(INFO) << "Read box-hierarchy";
			return bounding_box_hierarchy(primitives);
		}

		std::vector<std::shared_ptr<Hit>> find_all_hits(const Ray& ray) const override {
			return primitive->find_all_hits(ray);
		}

		bool find_first_positive_hit(const Ray& ray, Hit* hit) const override {
			return primitive->find_first_positive_hit(ray, hit);
		}

		math::Box bounding_box() const override { return primitive->bounding_box(); }
	};
}

Primitive raytracer::primitives::mesh_thijs(const std::string p)
{
	return Primitive(std::make_shared<MeshPrimitive>(p));
}
