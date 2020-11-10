#pragma once

#include "samplers/sampler.h"


namespace raytracer
{
	namespace samplers
	{
		Sampler multi_jittered(const int& n);
	}
}