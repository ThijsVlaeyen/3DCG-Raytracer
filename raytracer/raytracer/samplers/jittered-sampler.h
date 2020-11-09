#pragma once

#include "samplers/sampler.h"


namespace raytracer
{
	namespace samplers
	{
		Sampler jittered(const int& rows, const int& cols);
	}
}