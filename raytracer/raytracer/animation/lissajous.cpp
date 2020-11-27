#include "animation\lissajous.h"

using namespace animation;
using namespace math;
using namespace math;

Animation<math::Point3D> animation::lissajous_animation(animation::Duration duration, const double& xAmp, const double& yAmp, const double& zAmp, const double& xFreq, const double& yFreq, const double& zFreq, math::Angle& xPhase, math::Angle& yPhase, math::Angle& zPhase)
{
	auto double_animation = basic(0, 2 * atan(1) * 4, duration);

	std::function<math::Point3D(TimeStamp)> lambda = [xAmp, yAmp, zAmp, xFreq, yFreq, zFreq, xPhase, yPhase, zPhase, double_animation](TimeStamp now) -> math::Point3D {
		auto x = sin(Angle::radians(xFreq * double_animation(now)) + xPhase);
		auto y = sin(Angle::radians(yFreq * double_animation(now)) + yPhase);
		auto z = sin(Angle::radians(zFreq * double_animation(now)) + zPhase);

		return math::Point3D(xAmp * x, yAmp * y, zAmp * z);
	};

	return make_animation(math::from_lambda(lambda), duration);
}