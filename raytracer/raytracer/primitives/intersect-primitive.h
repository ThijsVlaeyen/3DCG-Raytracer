#pragma once

#include "primitives/primitive.h"
#include <vector>
#include <memory>


namespace raytracer
{
	namespace primitives
	{
		Primitive make_intersect(std::vector<Primitive>&);
	}
}