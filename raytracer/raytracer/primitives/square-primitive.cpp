#include "primitives/square-primitive.h"
#include "math/interval.h"


using namespace raytracer;
using namespace raytracer::primitives;
using namespace math;

namespace
{
	class SquareXYImplementation : public raytracer::primitives::_private_::PrimitiveImplementation
	{
	public:

		bool find_first_positive_hit(const Ray& ray, Hit* hit) const override
		{
			auto hits = find_all_hits(ray);
			if (hits.size() > 0) {
				auto found = hits.back();
				if (found->t > 0 && found->t < hit->t) {
					*hit = *found;
					return true;
				}
			}
			return false;
		}

		std::vector<std::shared_ptr<Hit>> find_all_hits(const Ray& ray) const override
		{
			std::vector<std::shared_ptr<Hit>> hits;
			double denom = ray.direction.dot(m_normal);

			if (denom != approx(0.0))
			{
				double numer = -((ray.origin - Point3D(0, 0, 0)).dot(m_normal));
				double t = numer / denom;
				if (bounding_box().contains(ray.at(t)))
				{
					auto hit = std::make_shared<Hit>();
					initialize_hit(hit.get(), ray, t);
					hits.push_back(hit);
				}
			}
			return hits;
		}

		Box bounding_box() const override
		{
			return Box(interval(-1.0, 1.0), interval(-1.0, 1.0), interval(-.01, .01));
		}

	protected:
		Vector3D m_normal = Vector3D(0, 0, 1);

		void initialize_hit(Hit* hit, const Ray& ray, double t) const
		{
			hit->t = t;
			hit->position = ray.at(hit->t);
			hit->local_position.xyz = hit->position;
			hit->local_position.uv = Point2D(hit->position.x(), hit->position.y());
			hit->normal = ray.origin.x() > 0 ? m_normal : -m_normal;
		}


	};
}

Primitive raytracer::primitives::xy_square()
{
	return Primitive(std::make_shared<SquareXYImplementation>());
}
