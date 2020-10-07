#include "animation/animation.h"
#include <functional>

using namespace animation;
using namespace math;


Animation<double> animation::basic(double from, double to, const Duration& duration)
{
    auto position_interval = interval(from, to);
    auto time_interval = interval(TimeStamp::zero(), TimeStamp::from_epoch(duration));

    std::function<double(TimeStamp)> lambda = [position_interval, time_interval](TimeStamp now) -> double {
        double t = time_interval.to_relative(now);
        return position_interval.from_relative(t);
    };

    return make_animation(from_lambda(lambda), duration);
}

Animation<Angle> animation::angle(Angle from, Angle to, const Duration& duration)
{
	auto position_interval = interval(from, to);
	auto time_interval = interval(TimeStamp::zero(), TimeStamp::from_epoch(duration));

	std::function<Angle(TimeStamp)> lambda = [position_interval, time_interval](TimeStamp now) -> Angle {
		double t = time_interval.to_relative(now);
		return position_interval.from_relative(t);
	};

	return make_animation(from_lambda(lambda), duration);
}
