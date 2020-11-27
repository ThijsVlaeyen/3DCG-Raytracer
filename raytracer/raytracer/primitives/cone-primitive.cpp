#include "primitives/cone-primitive.h"
#include "math/interval.h"

using namespace raytracer;
using namespace primitives;
using namespace math;

namespace
{
	class ConeImplementation : public primitives::_private_::PrimitiveImplementation
	{
		protected:
			const Vector3D cone_direction_;
			ConeImplementation(const Vector3D& direction) : cone_direction_(direction) {}
			void initialize_hit(Hit* hit, const Ray& ray, double t) const
			{
				const auto factor_x = cone_direction_.x() == 1 ? -1 : 1;
				const auto factor_y = cone_direction_.y() == 1 ? -1 : 1;
				const auto factor_z = cone_direction_.z() == 1 ? -1 : 1;

				hit->t = t;
				hit->position = ray.origin + ray.direction * t;
				hit->local_position.xyz = hit->position;

				if (factor_x == -1)
					hit->local_position.uv = Point2D(hit->position.y(), hit->position.z());
				if (factor_y == -1)
					hit->local_position.uv = Point2D(hit->position.x(), hit->position.z());
				if (factor_z == -1)
					hit->local_position.uv = Point2D(hit->position.x(), hit->position.y());

				hit->normal = Vector3D(
					factor_x * hit->position.x(),
					factor_y * hit->position.y(),
					factor_z * hit->position.z()
				).normalized();
			}
		public:
			Box bounding_box() const override
			{
				return Box(Interval<double>::infinite(), Interval<double>::infinite(), Interval<double>::infinite());
			}
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

			std::vector<std::shared_ptr<Hit>> find_all_hits(const Ray& ray) const override
			{
				// TODO: probably a nicer math way to do this
				const auto factor_x = cone_direction_.x() == 1 ? -1 : 1;
				const auto factor_y = cone_direction_.y() == 1 ? -1 : 1;
				const auto factor_z = cone_direction_.z() == 1 ? -1 : 1;

				std::vector<std::shared_ptr<Hit>> hits;
				const auto a =
					(factor_x * pow(ray.direction.x(), 2)) +
					(factor_y * pow(ray.direction.y(), 2)) +
					(factor_z * pow(ray.direction.z(), 2));

				const auto b =
					(factor_x * 2 * ray.origin.x() * ray.direction.x()) +
					(factor_y * 2 * ray.origin.y() * ray.direction.y()) +
					(factor_z * 2 * ray.origin.z() * ray.direction.z());

				const auto c =
					(factor_x * pow(ray.origin.x(), 2)) +
					(factor_y * pow(ray.origin.y(), 2)) +
					(factor_z * pow(ray.origin.z(), 2));

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
	};

	class ConeAlongYImplementation : public ConeImplementation
	{
		public:
			ConeAlongYImplementation() : ConeImplementation(Vector3D(0,1,0)){}
	};
}

Primitive primitives::cone_along_y()
{
	return Primitive(std::make_shared<ConeAlongYImplementation>());
}