#include "primitives/bounding-box-primitive.h"
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
	CREATE_PERFORMANCE_COUNTER(hit_count, "Hits");
	CREATE_PERFORMANCE_COUNTER(miss_count, "Misses");

	class BoundingBoxImplementation : public raytracer::primitives::_private_::PrimitiveImplementation
	{
	public:
		Primitive primitive;
		Box box;

		BoundingBoxImplementation(const Primitive primitive) : primitive(primitive), box(primitive->bounding_box()) {}

		std::vector<std::shared_ptr<Hit>> find_all_hits(const Ray& ray) const override {
			if (!box.is_hit_by(ray)) {
				INCREMENT_PERFORMANCE_COUNTER(miss_count);
				return std::vector<std::shared_ptr<Hit>>();
			}
			INCREMENT_PERFORMANCE_COUNTER(hit_count);
			return primitive->find_all_hits(ray);
		}

		math::Box bounding_box() const override
		{
			return box;
		}
	};
}

Primitive raytracer::primitives::bounding_box_accelerator(const Primitive primitive)
{
	return Primitive(std::make_shared<BoundingBoxImplementation>(primitive));
}
