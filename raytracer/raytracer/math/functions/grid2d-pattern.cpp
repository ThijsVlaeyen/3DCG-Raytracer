#include "math/functions/horizontal-lines-pattern.h"
#include "math/functions.h"
#include <cmath>

using namespace math;

Function<bool(const Point2D&)> math::functions::grid2d(double thickness)
{
	std::function<bool(const Point2D&)> function = [thickness](const Point2D& p)
	{
		auto y = p.y();
		auto x = p.x();
		return std::abs(y - round(y)) < thickness / 2 || std::abs(x - round(x)) < thickness / 2;
	};
	return from_lambda<bool, const Point2D&>(function);
}