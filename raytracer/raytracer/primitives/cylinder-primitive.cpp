#include "primitives/cylinder-primitive.h"
#include "math/interval.h"

using namespace raytracer;
using namespace primitives;
using namespace math;


namespace
{
	class CoordinateCylinderImplementation : public primitives::_private_::PrimitiveImplementation
	{
	protected:
		CoordinateCylinderImplementation() {}

		virtual void initialize_hit(Hit* hit, const Ray& ray, double t) const = 0;
		virtual std::vector<std::shared_ptr<Hit>> find_all_hits(const Ray& ray) const = 0;

	public:
		bool find_first_positive_hit(const Ray& ray, Hit* output_hit)  const override
		{
			auto hits = find_all_hits(ray);

			for (auto hit : hits)
			{
				if (hit->t > 0)
				{
					if (hit->t < output_hit->t)
					{
						*output_hit = *hit;
						return true;
					}
					return false;
				}
			}
			return false;
		}
	};


	class CylinderAlongYImplementation : public CoordinateCylinderImplementation {
	public:
		CylinderAlongYImplementation()
			: CoordinateCylinderImplementation()
		{
			// NOP
		}

		Box bounding_box() const override
		{
			return Box(Interval<double>::infinite(), Interval<double>::infinite(), interval(-0.01, 0.01));
		}

	protected:
		std::vector<std::shared_ptr<Hit>> find_all_hits(const Ray& ray) const override
		{
			std::vector<std::shared_ptr<Hit>> hits;

			const auto radius = 1;

			const auto O = Point2D(ray.origin.x(), ray.origin.z());
			auto delta = Vector2D(ray.direction.x(), ray.direction.z());
			const auto a = delta.dot(delta);
			const auto b = 2 * (O - Point2D(0, 0)).dot(delta);
			const auto c = (O - Point2D(0, 0)).dot(O - Point2D(0, 0)) - pow(radius, 2);
			const auto D = pow(b, 2) - (4 * a * c);

			if (D > 0)
			{
				auto hit1 = std::make_shared<Hit>();
				const auto t1 = (-b - sqrt(D)) / (2 * a);
				initialize_hit(hit1.get(), ray, t1);
				hits.push_back(hit1);

				auto hit2 = std::make_shared<Hit>();
				const auto t2 = (-b + sqrt(D)) / (2 * a);
				initialize_hit(hit2.get(), ray, t2);
				hits.push_back(hit2);
			}
			return hits;
		}

		void initialize_hit(Hit* hit, const Ray& ray, double t) const override
		{
			const auto radius = 1;

			hit->t = t;
			hit->position = ray.origin + ray.direction * t;
			hit->local_position.xyz = hit->position;
			hit->local_position.uv = Point2D(hit->position.x(), hit->position.z());
			hit->normal = Vector3D(0, hit->position.x() / radius, hit->position.z() / radius);
		}
	};
}




Primitive primitives::cylinder_along_y()
{
	return Primitive(std::make_shared<CylinderAlongYImplementation>());
}

