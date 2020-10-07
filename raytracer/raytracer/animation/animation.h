#pragma once

#include "math/function.h"
#include "math/point.h"
#include "math/interval.h"
#include "math/functions/easing-functions.h"
#include "math/angle.h"
#include "animation/duration.h"
#include "animation/time-stamp.h"
#include "easylogging++.h"
#include <assert.h>


namespace animation
{
    template<typename T>
    class Animation
    {
    public:
        Animation(math::Function<T(TimeStamp)> function, animation::Duration duration)
            : m_function(function), m_duration(duration) { }

        static Animation<T> empty()
        {
            std::function<T(TimeStamp)> lambda = [](TimeStamp) -> T {
                LOG(ERROR) << "Cannot run empty animation";

                abort();
            };

            return Animation<T>(from_lambda(lambda), Duration::zero());
        }

        T operator ()(TimeStamp t) const
        {
            return m_function(t);
        }

        math::Function<T(TimeStamp)> function() const
        {
            return m_function;
        }

        Duration duration() const
        {
            return m_duration;
        }

    private:
        math::Function<T(TimeStamp)> m_function;
        animation::Duration m_duration;
    };

    inline math::Function<TimeStamp(double)> seconds_to_timestamp()
    {
        std::function<TimeStamp(double)> lambda = [](double seconds) -> TimeStamp {
            return TimeStamp::from_epoch(Duration::from_seconds(seconds));
        };

        return math::from_lambda<TimeStamp, double>(lambda);
    }

    inline math::Function<double(TimeStamp)> timestamp_to_seconds()
    {
        std::function<double(TimeStamp)> lambda = [](TimeStamp ts) -> double {
            return ts.seconds();
        };

        return math::from_lambda<double, TimeStamp>(lambda);
    }

    template<typename T>
    Animation<T> make_animation(math::Function<T(TimeStamp)> function, animation::Duration duration)
    {
        return Animation<T>(function, duration);
    }

    template<typename T>
    Animation<T> preprocess(math::Function<TimeStamp(TimeStamp)> function, Animation<T> animation)
    {
        return make_animation<T>(function >> animation.function(), animation.duration());
    }

    template<typename T1, typename T2>
    Animation<T2> postprocess(Animation<T1> animation, math::Function<T2(T1)> function)
    {
        return make_animation<T2>(animation.function() >> function, animation.duration());
    }

    Animation<double> basic(double, double, const Duration&);
    Animation<math::Angle> angle(math::Angle, math::Angle, const Duration&);
}
