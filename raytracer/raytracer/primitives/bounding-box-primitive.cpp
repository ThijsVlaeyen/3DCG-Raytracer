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
	class BoundingBoxImplementation : public raytracer::primitives::_private_::PrimitiveImplementation
	{
	public:
		Primitive primitive;
		Box box;

		BoundingBoxImplementation(const Primitive primitive) : primitive(primitive), box(primitive->bounding_box()) {}

		std::vector<std::shared_ptr<Hit>> find_all_hits(const Ray& ray) const override {
			if (!box.is_hit_positively_by(ray)) {
				return std::vector<std::shared_ptr<Hit>>();
			}
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
