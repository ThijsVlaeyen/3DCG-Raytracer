#pragma once
#include "samplers/sampler.h"


namespace raytracer
{
	namespace samplers
	{
		Sampler stratified(const int& rows, const int& cols);
	}
}