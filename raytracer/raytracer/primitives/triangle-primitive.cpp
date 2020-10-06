#include "primitives/triangle-primitive.h"
#include "util/misc.h"
#include "math/coordinate-systems.h"
#include "math/quadratic_equation.h"
#include "performance/performance.h"
#include <assert.h>

using namespace raytracer;
using namespace raytracer::primitives;
using namespace math;


namespace
{
	class TriangleImplementation : public raytracer::primitives::_private_::PrimitiveImplementation
	{
	public:
		Point3D x, y, z;
		Vector3D normal;

		TriangleImplementation(const Point3D x, const Point3D y, const Point3D z)
			: x(x), y(y), z(z) {
			normal = (y - x).cross(z - x).normalized();
		}

		std::vector<std::shared_ptr<Hit>> find_all_hits(const Ray& ray) const override {
			std::vector<std::shared_ptr<Hit>> hits;


			// Formula found bottom: http://3dcg.leone.ucll.be/reference/primitives/triangle/explanations.html
			auto t = (x - ray.origin).dot(normal) / ray.direction.dot(normal);
			auto h = ray.origin + ray.direction * t;
			if ((y - x).cross(h - x).dot(normal) < 0) return hits;
			if ((z - y).cross(h - y).dot(normal) < 0) return hits;
			if ((x - z).cross(h - z).dot(normal) < 0) return hits;
			auto hit = std::make_shared<Hit>();

			// Init hit
			initialize_hit(hit.get(), ray, t);
			// Put hit in vector
			hits.push_back(hit);
			// Return hit list
			return hits;
		}

		math::Box bounding_box() const override
		{
			auto range = interval(-1.0, 1.0);
			return Box(range, range, range);
		}

	private:
		void initialize_hit(Hit* hit, const Ray& ray, double t) const
		{
			hit->t = t;
			hit->position = ray.at(t);
			hit->local_position.xyz = hit->position;
			hit->local_position.uv = Point2D(0, 0);
			hit->normal = normal;
		}
	};
}

Primitive raytracer::primitives::triangle(const Point3D x, const Point3D y, const Point3D z)
{
	return Primitive(std::make_shared<TriangleImplementation>(x, y, z));
}
