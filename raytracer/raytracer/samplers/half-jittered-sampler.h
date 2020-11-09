#pragma once

#include "samplers/sampler.h"


namespace raytracer
{
	namespace samplers
	{
		Sampler half_jittered(const int& rows, const int& cols);
	}
}