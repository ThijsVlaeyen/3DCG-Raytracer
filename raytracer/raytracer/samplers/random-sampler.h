#pragma once
#include "samplers/sampler.h"

namespace raytracer
{
	namespace samplers
	{
		/// Creates a sampler that always picks the center of the given rectangle
		Sampler random(int count);
	}
}