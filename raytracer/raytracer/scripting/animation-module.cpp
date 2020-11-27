#ifndef EXCLUDE_SCRIPTING

#include "scripting/animation-module.h"
#include "scripting/scripting-util.h"
#include "raytracers/scene.h"
#include "animation/animations.h"
#include "math/angle.h"
#include "math/functions.h"


using namespace chaiscript;
using namespace raytracer;
using namespace math;
using namespace math::functions;
using namespace imaging;
using namespace animation;


namespace
{
    struct AnimationLibrary
    {
        Animation<Point3D> point_animation(const Point3D& from, const Point3D& to, Duration duration) const
        {
            return animation::animate(from, to, duration);
        }

        Animation<double> double_animation(double from, double to, Duration duration) const
        {
            return animation::animate(from, to, duration);
        }

        Animation<Angle> angle_animation(Angle from, Angle to, Duration duration) const
        {
            return animation::animate(from, to, duration);
        }

        template<typename T>
        Animation<T> ease_animation(Animation<T> a, EasingFunction f) const
        {
            return ease(a, f);
        }

        Animation<Point3D> lissajous(animation::Duration duration, const double& xAmp, const double& yAmp, const double& zAmp, const double& xFreq, const double& yFreq, const double& zFreq, math::Angle& xPhase, math::Angle& yPhase, math::Angle& zPhase) const
        {
            return animation::lissajous_animation(duration, xAmp, yAmp, zAmp, xFreq, yFreq, zFreq, xPhase, yPhase, zPhase);
        }

        Animation<Point3D> mapped_lissajous(const std::map<std::string, Boxed_Value>& argument_map) const
        {
            START_ARGUMENTS(argument_map);
            OPTIONAL_ARGUMENT(Duration, duration, 1_s);
            OPTIONAL_ARGUMENT(int, x_amplitude, 0);
            OPTIONAL_ARGUMENT(int, y_amplitude, 0);
            OPTIONAL_ARGUMENT(int, z_amplitude, 0);
            OPTIONAL_ARGUMENT(int, x_frequency, 0);
            OPTIONAL_ARGUMENT(int, y_frequency, 0);
            OPTIONAL_ARGUMENT(int, z_frequency, 0);
            OPTIONAL_ARGUMENT(Angle, x_phase, 0_degrees);
            OPTIONAL_ARGUMENT(Angle, y_phase, 0_degrees);
            OPTIONAL_ARGUMENT(Angle, z_phase, 0_degrees);
            END_ARGUMENTS();

            return animation::lissajous_animation(duration, x_amplitude, y_amplitude, z_amplitude, x_frequency, y_frequency, z_frequency, x_phase, y_phase, z_phase);
        }
    };

    Duration seconds(double s)
    {
        return Duration::from_seconds(s);
    }

    Animation<std::shared_ptr<Scene>> make_scene_animation(std::function<std::shared_ptr<Scene>(TimeStamp)> function, Duration duration)
    {
        return Animation<std::shared_ptr<Scene>>(from_lambda(function), duration);
    }
}

ModulePtr raytracer::scripting::_private_::create_animation_module()
{
    auto module = std::make_shared<chaiscript::Module>();

    util::register_to_string<Duration>(*module);
    util::register_to_string<TimeStamp>(*module);

    auto animation_library = std::make_shared<AnimationLibrary>();
    module->add_global_const(chaiscript::const_var(animation_library), "Animations");

#define BIND_AS(INTERNAL, EXTERNAL)                 module->add(fun(&AnimationLibrary::INTERNAL), #EXTERNAL)
#define BIND(NAME)                                  BIND_AS(NAME, NAME)
    BIND_AS(double_animation, animate);
    BIND_AS(point_animation, animate);
    BIND_AS(angle_animation, animate);
    BIND_AS(ease_animation<double>, ease);
    BIND_AS(ease_animation<Angle>, ease);
    BIND_AS(ease_animation<Point3D>, ease);
    BIND_AS(lissajous, animate);
    BIND_AS(mapped_lissajous, lissajous_animation);
#undef BIND
#undef BIND_AS

    module->add(fun(&seconds), "seconds");
    module->add(fun([](TimeStamp t, Duration d) { return t + d; }), "+");
    module->add(fun([](Duration d, TimeStamp t) { return t + d; }), "+");
    module->add(fun(&Animation<Point3D>::operator()), "[]");
    module->add(fun(&Animation<Angle>::operator()), "[]");
    module->add(fun(&Animation<double>::operator()), "[]");
    module->add(fun(&make_scene_animation), "scene_animation");

    return module;
}

#endif
