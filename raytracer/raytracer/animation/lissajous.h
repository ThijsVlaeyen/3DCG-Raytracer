#pragma once
#include "animation/animation.h"

namespace animation
{
	Animation<math::Point3D> lissajous_animation(animation::Duration duration, const double& xAmp, const double& yAmp, const double& zAmp, const double& xFreq,
		const double& yFreq, const double& zFreq, math::Angle& xPhase, math::Angle& yPhase, math::Angle& zPhase);
}