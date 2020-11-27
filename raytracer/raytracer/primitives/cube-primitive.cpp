#include "cube-primitive.h"
#include "primitives/square-primitive.h"
#include "math/interval.h"
#include "transformer-primitive.h"


using namespace raytracer;
using namespace primitives;
using namespace math;

namespace
{
	class CubeImplementation : public primitives::_private_::PrimitiveImplementation
	{
	public:
		std::vector<std::shared_ptr<Hit>> find_all_hits(const Ray& ray) const override
		{
			std::vector<std::shared_ptr<Hit>> hits;

			//xz_square
			auto xz_hits = translate(Vector3D(0, 1, 0), xz_square())->find_all_hits(ray);
			hits.insert(hits.end(), xz_hits.begin(), xz_hits.end());
			auto xz2_hits = translate(Vector3D(0, -1, 0), xz_square())->find_all_hits(ray);
			hits.insert(hits.end(), xz2_hits.begin(), xz2_hits.end());

			//xy_square
			auto xy_hits = translate(Vector3D(0, 0, 1), xy_square())->find_all_hits(ray);
			hits.insert(hits.end(), xy_hits.begin(), xy_hits.end());
			auto xy2_hits = translate(Vector3D(0, 0, -1), xy_square())->find_all_hits(ray);
			hits.insert(hits.end(), xy2_hits.begin(), xy2_hits.end());

			//yz_square
			auto yz_hits = translate(Vector3D(1, 0, 0), yz_square())->find_all_hits(ray);
			hits.insert(hits.end(), yz_hits.begin(), yz_hits.end());
			auto yz2_hits = translate(Vector3D(-1, 0, 0), yz_square())->find_all_hits(ray);
			hits.insert(hits.end(), yz2_hits.begin(), yz2_hits.end());
			
			std::sort(hits.begin(), hits.end(), [](const std::shared_ptr<Hit>& a, const std::shared_ptr<Hit>& b) { return a->t < b->t; });

			return hits;
		}

		Box bounding_box() const override
		{
			// Create a [-1, 1] x [-1, 1] x [-1, 1] box.
			auto range = interval(-1.0, 1.0);

			return Box(range, range, range);
		}

		bool find_first_positive_hit(const Ray& ray, Hit* output_hit) const override
		{
			auto hits = find_all_hits(ray);

			for (auto hit : hits)
			{
				// Find first positive hit
				if (hit->t > 0)
				{
					if (hit->t < output_hit->t)
					{
						// Overwrite hit with new hit
						*output_hit = *hit;
						return true;

					}
					// First positive hit is farther away than already existing hit
					return false;
				}
			}
			return false;
		}
	};
}

Primitive primitives::cube()
{
	return Primitive(std::make_shared<CubeImplementation>());
}