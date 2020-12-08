#include "quintic-function.h"
#include <cassert>
#include "math/interval.h"

using namespace math::functions;
using namespace math;


EasingFunction easing::quintic_in()
{
	std::function<double(double)> lambda = [](double t)
	{
		assert(interval(0.0, 1.0).contains(t));
		return t * t * t * t * t;
	};

	return from_lambda(lambda);
}

EasingFunction easing::quintic_out()
{
	std::function<double(double)> lambda = [](double t)
	{
		assert(interval(0.0, 1.0).contains(t));
		t--;
		return t * t * t * t * t + 1;
	};

	return from_lambda(lambda);
}

EasingFunction easing::quintic_inout()
{
	std::function<double(double)> lambda = [](double t)
	{
		assert(interval(0.0, 1.0).contains(t));
		t /= 0.5;
		if (t < 1) return 0.5 * t * t * t * t * t;
		t -= 2;
		return 0.5 * (t * t * t * t * t + 2);
	};

	return from_lambda(lambda);
}


