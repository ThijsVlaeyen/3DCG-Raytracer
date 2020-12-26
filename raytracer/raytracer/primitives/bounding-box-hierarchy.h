#pragma once

#include "primitives/primitive.h"

using namespace math;

namespace raytracer
{
	namespace primitives
	{
		Primitive bounding_box_hierarchy(const std::vector<Primitive>&);
	}
}
