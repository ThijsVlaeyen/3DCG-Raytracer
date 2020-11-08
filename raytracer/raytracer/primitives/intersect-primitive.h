#pragma once

#include "primitives/primitive.h"
#include <memory>


namespace raytracer
{
	namespace primitives
	{
		Primitive make_intersect(Primitive child1, Primitive child2);
	}
}