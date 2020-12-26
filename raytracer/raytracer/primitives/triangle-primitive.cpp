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

		TriangleImplementation(const Point3D x, const Point3D y, const Point3D z) : x(x), y(y), z(z) {
			normal = (y - x).cross(z - x).normalized();
		}

		bool find_first_positive_hit(const Ray& ray, Hit* hit) const override {
			double t = (x - ray.origin).dot(normal) / ray.direction.dot(normal);
			if (t < 0 || t >= hit->t) return false;
			Point3D h = ray.origin + ray.direction * t;
			if ((y - x).cross(h - x).dot(normal) < 0) return false;
			if ((z - y).cross(h - y).dot(normal) < 0) return false;
			if ((x - z).cross(h - z).dot(normal) < 0) return false;
			initialize_hit(hit, ray, t);
			return true;
		}

		std::vector<std::shared_ptr<Hit>> find_all_hits(const Ray& ray) const override {
			std::vector<std::shared_ptr<Hit>> hits;
			std::shared_ptr<Hit> hit;

			// Formula found bottom: http://3dcg.leone.ucll.be/reference/primitives/triangle/explanations.html
			auto t = (x - ray.origin).dot(normal) / ray.direction.dot(normal);
			auto h = ray.origin + ray.direction * t;
			if ((y - x).cross(h - x).dot(normal) < 0) return hits;
			if ((z - y).cross(h - y).dot(normal) < 0) return hits;
			if ((x - z).cross(h - z).dot(normal) < 0) return hits;

			// Init hit
			initialize_hit(hit.get(), ray, t);
			// Put hit in vector
			hits.push_back(hit);
			// Return hit list
			return hits;
		}

		math::Box bounding_box() const override
		{
			double x_min = std::min({ x.x(), y.x(),z.x() });
			double y_min = std::min({ x.y(), y.y(),z.y() });
			double z_min = std::min({ x.z(), y.z(),z.z() });

			double x_max = std::max({ x.x(), y.x(),z.x() });
			double y_max = std::max({ x.y(), y.y(),z.y() });
			double z_max = std::max({ x.z(), y.z(),z.z() });

			Interval<double> x = interval(x_min, x_max);
			Interval<double> y = interval(y_min, y_max);
			Interval<double> z = interval(z_min, z_max);

			return Box(x, y, z);
		}

	private:
		void initialize_hit(Hit* hit, const Ray& ray, double t) const
		{
			// UPDATE HIT OBJECT
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
