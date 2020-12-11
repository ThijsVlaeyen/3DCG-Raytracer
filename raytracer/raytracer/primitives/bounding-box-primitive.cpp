#include "primitives/bounding-box-primitive.h"
#include "util/misc.h"
#include "math/coordinate-systems.h"
#include "math/quadratic_equation.h"
#include "performance/performance.h"
#include <assert.h>

using namespace raytracer;
using namespace math;

namespace
{
	class BoundingBoxAccelerator : public primitives::_private_::PrimitiveImplementation
	{
	public:
		const Primitive p;
		const Box bb_child;

		BoundingBoxAccelerator(const Primitive& primitive) :p(primitive), bb_child(primitive->bounding_box()) {}

		std::vector<std::shared_ptr<Hit>> find_all_hits(const Ray& ray) const override
		{
			// first check if it intersects the box.
			if (!bb_child.is_hit_positively_by(ray))
			{
				// return if no intersection.
				return std::vector<std::shared_ptr<Hit>>();
			}
			// If intersection, find all the hits.
			return p->find_all_hits(ray);
		}

		bool find_first_positive_hit(const Ray& ray, Hit* output_hit) const override
		{
			// first check if it intersects the box.
			if (!bb_child.is_hit_positively_by(ray))
			{
				// return if no intersection.
				return false;
			}
			// find the first positive hit.
			return p->find_first_positive_hit(ray, output_hit);
		}

		Box bounding_box() const override
		{
			return bb_child;
		}
	};
}

Primitive primitives::bounding_box_accelerator(const Primitive& primitive)
{
	return Primitive(std::make_shared<BoundingBoxAccelerator>(primitive));
}