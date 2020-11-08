#include "primitives/intersect-primitive.h"
#include "easylogging++.h"
#include <algorithm>

using namespace raytracer;
using namespace primitives;
using namespace math;

namespace
{
	class BinaryIntersectImplementation : public primitives::_private_::PrimitiveImplementation
	{
	public:
		BinaryIntersectImplementation(Primitive child1, Primitive child2) 
			: m_child1(child1), m_child2(child2)
		{
			//NOP
		}

		bool find_first_positive_hit(const Ray& ray, Hit* outhit) const override
		{
            auto hits = find_all_hits(ray);
            for (auto hit : hits)
            {
                if (hit->t > 0)
                {
                    if (hit->t < outhit->t)
                    {
                        *outhit = *hit;
                        return true;
                    }
                    return false;
                }

            }
            return false;

		}

		std::vector<std::shared_ptr<Hit>> find_all_hits(const Ray& ray) const override
		{
			std::vector<std::shared_ptr<Hit>> result;

			bool inside1 = false;
			bool inside2 = false;
			bool was_inside_intersection = false;

			// Collect children's hits
			auto hits1 = m_child1->find_all_hits(ray);
			auto hits2 = m_child2->find_all_hits(ray);

			// Append hits at infinity
			hits1.push_back(std::make_shared<Hit>());
			hits2.push_back(std::make_shared<Hit>());

			// Start iterating over hit lists
			auto i1 = hits1.begin();
			auto i2 = hits2.begin();

			while (i1 + 1 != hits1.end() || i2 + 1 != hits2.end())
			{
				// Get hits
				auto hit1 = *i1;
				auto hit2 = *i2;
				std::shared_ptr<Hit> active_hit = nullptr;

				// Get t-values of hits
				double t1 = hit1->t;
				double t2 = hit2->t;

				// Find closest hit
				if (t1 < t2)
				{
					// If we were inside child1, we're now outside, and vice versa
					inside1 = !inside1;

					// Save hit for later
					active_hit = hit1;

					// Move to next hit
					++i1;
				}
				else if (t1 > t2)
				{
					// If we were inside child2, we're now outside, and vice versa
					inside2 = !inside2;

					// Save hit for later
					active_hit = hit2;

					// Move to next hit
					++i2;
				}
				else
				{
					inside1 = !inside1;
					inside2 = !inside2;

					active_hit = hit1;

					++i1;
					++i2;
				}

				// We're inside the intersection if we're inside both of the children
				bool inside_intersection = inside1 && inside2;

				// Check if there was a change
				if (was_inside_intersection != inside_intersection)
				{
					// The active hit is important; add it to the result list
					result.push_back(active_hit);

					// Overwrite value
					was_inside_intersection = inside_intersection;
				}
			}

			return result;
		}

        math::Box bounding_box() const override
        {
            return m_child1->bounding_box().intersect(m_child2->bounding_box());
        }
	private:
		Primitive m_child1;
		Primitive m_child2;
	};

    Primitive build_intersect_tree(std::vector<Primitive>& children, int from, int to)
    {
        assert(from < to);

        if (to - from == 1)
        {
            return children[from];
        }
        else
        {
            int middle = (from + to) / 2;

            auto left = build_intersect_tree(children, from, middle);
            auto right = build_intersect_tree(children, middle, to);
        }
    }
}

Primitive primitives::make_intersect(Primitive child1, Primitive child2)
{
    return Primitive(std::make_shared<BinaryIntersectImplementation>(child1, child2));
}