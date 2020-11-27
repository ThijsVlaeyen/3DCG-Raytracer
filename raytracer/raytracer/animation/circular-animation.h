#pragma once
#include "animation/animation.h"

namespace animation
{
	Animation<math::Point3D> circular(const math::Point3D& point, const math::Point3D& center, const math::Vector3D& rotation_axis, const math::Interval<math::Angle>& angle_interval, const Duration& duration);
}