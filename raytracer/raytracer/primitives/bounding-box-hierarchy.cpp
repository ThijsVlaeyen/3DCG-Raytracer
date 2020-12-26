#include "bounding-box-hierarchy.h"
#include "bounding-box-primitive.h"
#include "util/misc.h"
#include "math/coordinate-systems.h"
#include "math/quadratic_equation.h"
#include "performance/performance.h"
#include "easylogging++.h"
#include <assert.h>

using namespace raytracer;
using namespace raytracer::primitives;
using namespace math;


namespace
{
	class BoundingBoxHierarchyImplementation : public raytracer::primitives::_private_::PrimitiveImplementation
	{
	public:
		std::vector<Primitive> primitives;
		Box box;

		BoundingBoxHierarchyImplementation(const std::vector<Primitive>& p) : primitives(p), box(merge_bounding_boxes(p)) {}

		std::vector<std::shared_ptr<Hit>> find_all_hits(const Ray& ray) const override {
			if (!box.is_hit_positively_by(ray)) return std::vector<std::shared_ptr<Hit>>();
			std::vector<std::shared_ptr<Hit>> hits;
			for (Primitive p : primitives)
			{
				std::vector<std::shared_ptr<Hit>> h = p->find_all_hits(ray);
				hits.insert(hits.begin(), h.begin(), h.end());
			}
			return hits;
		}

		bool find_first_positive_hit(const Ray& ray, Hit* hit) const override {
			if (!box.is_hit_positively_by(ray)) return false;
			bool b = false;
			for (Primitive p : primitives)
			{
				if (p->find_first_positive_hit(ray, hit)) b = true;
			}
			return b;
		}

		math::Box bounding_box() const override { return box; }

	private:
		math::Box merge_bounding_boxes(const std::vector<Primitive>& p) const {
			Box b(Box::empty());
			for (Primitive primitive : p)
			{
				b = b.merge(primitive->bounding_box());
			}
			return b;
		}
	};

}

Primitive raytracer::primitives::bounding_box_hierarchy(const std::vector<Primitive>& p)
{
	return Primitive(std::make_shared<BoundingBoxHierarchyImplementation>(p));
}
