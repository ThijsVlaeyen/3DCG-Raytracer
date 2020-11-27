#include "animation/circular-animation.h"
#include "math/matrix.h"
#include "data-structures/array.h"

using namespace animation;

Animation<math::Point3D> animation::circular(const math::Point3D& point, const math::Point3D& center, const math::Vector3D& rotation_axis, const math::Interval<math::Angle>& angle_interval, const Duration& duration)
{
	auto double_animation = basic(0.0, 1.0, duration);

	std::function<math::Point3D(TimeStamp)> lambda = [point, center, rotation_axis, angle_interval, double_animation](TimeStamp now) -> math::Point3D {
		const auto angle = angle_interval.from_relative(double_animation(now));

		const auto r = sqrt(pow(center.x() - point.x(), 2) + pow(center.y() - point.y(), 2) + pow(center.z() - point.z(), 2));

		const auto x = r * cos(angle);
		const auto y = r * sin(angle);

		auto u_axis = rotation_axis.normalized();
		auto v_axis = math::Vector3D(point.x() - center.x(), point.y() - center.y(), point.z() - center.z()).normalized();
		auto w_axis = u_axis.cross(v_axis).normalized();

		const auto new_x = u_axis.x() * x + u_axis.y() * y;
		const auto new_y = v_axis.x() * x + v_axis.y() * y;
		const auto new_z = w_axis.x() * x + w_axis.y() * y;

		return math::Point3D(new_x + center.x(), new_y + center.y(), new_z + center.z());
	};

	return make_animation(from_lambda(lambda), duration);
}