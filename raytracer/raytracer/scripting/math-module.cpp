#ifndef EXCLUDE_SCRIPTING

#include "scripting/math-module.h"
#include "scripting/scripting-util.h"
#include "math/point.h"
#include "math/vector.h"
#include "math/rectangle3d.h"
#include "math/functions.h"
#include "math/functions/easing-functions.h"


using namespace chaiscript;
using namespace raytracer;
using namespace math;
using namespace math::functions;


namespace
{
    Point2D create_point2d(double x, double y)
    {
        return Point2D(x, y);
    }

    Point3D create_point3d(double x, double y, double z)
    {
        return Point3D(x, y, z);
    }

    Vector2D create_vector2d(double x, double y)
    {
        return Vector2D(x, y);
    }

    Vector3D create_vector3d(double x, double y, double z)
    {
        return Vector3D(x, y, z);
    }

    Rectangle3D create_rectangle3d(const Point3D& origin, const Vector3D& x_axis, const Vector3D& y_axis)
    {
        return Rectangle3D(origin, x_axis, y_axis);
    }

    Interval<double> scalar_interval(double lower, double upper)
    {
        return interval(lower, upper);
    }

    Interval<Angle> angle_interval(Angle lower, Angle upper)
    {
        return interval(lower, upper);
    }

    struct PointFactories
    {
        Point2D cartesian2d(double x, double y) const                           { return Point2D::cartesian(x, y); }
        Point2D polar(double radius, Angle theta) const                         { return Point2D::polar(radius, theta); }
        Point3D cartesian3d(double x, double y, double z) const                 { return Point3D::cartesian(x, y, z); }
        Point3D spherical(double radius, Angle azimuth, Angle elevation) const  { return Point3D::spherical(radius, azimuth, elevation); }
        Point3D cylindrical_x(double radius, Angle azimuth, double x) const     { return Point3D::cylindrical_x(radius, azimuth, x); }
        Point3D cylindrical_y(double radius, Angle azimuth, double y) const     { return Point3D::cylindrical_y(radius, azimuth, y); }
        Point3D cylindrical_z(double radius, Angle azimuth, double z) const     { return Point3D::cylindrical_z(radius, azimuth, z); }
    };

    struct VectorFactories
    {
        Vector2D cartesian2d(double x, double y) const                          { return Vector2D::cartesian(x, y); }
        Vector2D polar(double radius, Angle theta) const                        { return Vector2D::polar(radius, theta); }
        Vector3D cartesian3d(double x, double y, double z) const                { return Vector3D::cartesian(x, y, z); }
        Vector3D spherical(double radius, Angle azimuth, Angle elevation) const { return Vector3D::spherical(radius, azimuth, elevation); }
        Vector3D cylindrical_x(double radius, Angle azimuth, double x) const    { return Vector3D::cylindrical_x(radius, azimuth, x); }
        Vector3D cylindrical_y(double radius, Angle azimuth, double y) const    { return Vector3D::cylindrical_y(radius, azimuth, y); }
        Vector3D cylindrical_z(double radius, Angle azimuth, double z) const    { return Vector3D::cylindrical_z(radius, azimuth, z); }
    };

    struct EasingLibrary
    {
        EasingFunction linear() const { return math::functions::easing::linear(); }
        EasingFunction quadratic_in() const { return math::functions::easing::quadratic_in(); }
        EasingFunction quadratic_out() const { return math::functions::easing::quadratic_out(); }
        EasingFunction quadratic_inout() const { return math::functions::easing::quadratic_inout(); }
        EasingFunction quintic_in() const { return math::functions::easing::quintic_in(); }
        EasingFunction quintic_out() const { return math::functions::easing::quintic_out(); }
        EasingFunction quintic_inout() const { return math::functions::easing::quintic_inout(); }
    };

    void add_points_and_vectors(Module& module)
    {
        raytracer::scripting::util::register_type<math::Point2D>(module, "Point2D");
        raytracer::scripting::util::register_type<math::Point3D>(module, "Point3D");
        raytracer::scripting::util::register_type<math::Vector2D>(module, "Vector2D");
        raytracer::scripting::util::register_type<math::Vector3D>(module, "Vector3D");

        raytracer::scripting::util::register_to_string<math::Point2D>(module);
        raytracer::scripting::util::register_to_string<math::Vector2D>(module);
        raytracer::scripting::util::register_to_string<math::Point3D>(module);
        raytracer::scripting::util::register_to_string<math::Vector3D>(module);

        raytracer::scripting::util::register_assignment<math::Point2D>(module);
        raytracer::scripting::util::register_assignment<math::Vector2D>(module);
        raytracer::scripting::util::register_assignment<math::Point3D>(module);
        raytracer::scripting::util::register_assignment<math::Vector3D>(module);

        module.add(fun(create_point3d), "pos");
        module.add(fun(create_point2d), "pos");
        module.add(fun(create_vector3d), "vec");
        module.add(fun(create_vector2d), "vec");
        
        module.add(fun([](const Vector2D& u, const Vector2D& v) { return u + v; }), "+");
        module.add(fun([](const Point2D& p, const Vector2D& v) { return p + v; }), "+");
        module.add(fun([](const Vector2D& u, const Point2D& p) { return u + p; }), "+");
        module.add(fun([](const Vector2D& u, const Vector2D& v) { return u - v; }), "-");
        module.add(fun([](const Point2D& p, const Vector2D& v) { return p - v; }), "-");
        module.add(fun([](const Point2D& p, const Point2D& q) { return p - q; }), "-");

        module.add(fun([](const Vector3D& u, const Vector3D& v) { return u + v; }), "+");
        module.add(fun([](const Point3D& p, const Vector3D& v) { return p + v; }), "+");
        module.add(fun([](const Vector3D& u, const Point3D& p) { return u + p; }), "+");        
        module.add(fun([](const Vector3D& u, const Vector3D& v) { return u - v; }), "-");
        module.add(fun([](const Point3D& p, const Vector3D& v) { return p - v; }), "-");
        module.add(fun([](const Point3D& p, const Point3D& q) { return p - q; }), "-");

        module.add(fun(&Vector2D::norm), "norm");
        module.add(fun(&Vector2D::normalize), "normalize");
        module.add(fun(&Vector2D::normalized), "normalized");
        module.add(fun([](const Point2D& p, const Point2D& q) -> double { return math::distance(p, q); }), "distance");

        module.add(fun(&Vector3D::norm), "norm");
        module.add(fun(&Vector3D::normalize), "normalize");
        module.add(fun(&Vector3D::normalized), "normalized");
        module.add(fun([](const Point3D& p, const Point3D& q) -> double { return math::distance(p, q); }), "distance");

        auto point_factories = std::make_shared<PointFactories>();
        module.add_global_const(chaiscript::const_var(point_factories), "Pos");
        auto vector_factories = std::make_shared<VectorFactories>();
        module.add_global_const(chaiscript::const_var(vector_factories), "Vec");

#       define BIND(NAME)    module.add(fun(&PointFactories::NAME), #NAME); module.add(fun(&VectorFactories::NAME), #NAME)
        BIND(cartesian2d);
        BIND(polar);
        BIND(cartesian3d);
        BIND(spherical);
        BIND(cylindrical_x);
        BIND(cylindrical_y);
        BIND(cylindrical_z);
#       undef BIND
    }

    void add_angle(Module& module)
    {
        raytracer::scripting::util::register_type<math::Angle>(module, "Angle");
        raytracer::scripting::util::register_to_string<math::Angle>(module);
        raytracer::scripting::util::register_assignment<math::Angle>(module);

        module.add(fun([](double x) { return Angle::degrees(x); }), "degrees");
        module.add(fun([](double x) { return Angle::radians(x); }), "radians");
        module.add(fun([](Angle a, Angle b) { return a + b; }), "+");
        module.add(fun([](Angle a, Angle b) { return a - b; }), "-");
        module.add(fun([](Angle a) { return -a; }), "-");
        module.add(fun([](Angle a, double constant) { return a * constant; }), "*");
        module.add(fun([](double constant, Angle a) { return constant * a; }), "*");
        module.add(fun([](Angle a, double constant) { return a / constant; }), "/");
        module.add(fun([](Angle a, Angle b) { return a - b; }), "-");
        module.add(fun([](Angle a) { return sin(a); }), "sin");
        module.add(fun([](Angle a) { return cos(a); }), "cos");
    }

    void add_interval(Module& module)
    {
        raytracer::scripting::util::register_type<math::Interval<double>>(module, "DoubleInterval");
        raytracer::scripting::util::register_to_string<math::Interval<double>>(module);
        raytracer::scripting::util::register_assignment<math::Interval<double>>(module);

        raytracer::scripting::util::register_type<math::Interval<Angle>>(module, "AngleInterval");
        raytracer::scripting::util::register_to_string<math::Interval<Angle>>(module);
        raytracer::scripting::util::register_assignment<math::Interval<Angle>>(module);

        module.add(fun(&scalar_interval), "interval");
        module.add(fun(&angle_interval), "interval");
    }

    void add_rectangle3d(Module& module)
    {
        module.add(fun(&create_rectangle3d), "rect3d");
    }

    void add_easing(Module& module)
    {
        auto easing_library = std::make_shared<EasingLibrary>();
        module.add_global_const(chaiscript::const_var(easing_library), "Easing");

        # define BIND(NAME)  module.add(fun(&EasingLibrary::NAME), #NAME)
            BIND(linear);
            BIND(quadratic_in);
            BIND(quadratic_out);
            BIND(quadratic_inout);
            BIND(quintic_in);
            BIND(quintic_out);
            BIND(quintic_inout);
        # undef BIND
    }
}

ModulePtr raytracer::scripting::_private_::create_math_module()
{
    auto module = std::make_shared<chaiscript::Module>();

    add_points_and_vectors(*module);
    add_rectangle3d(*module);
    add_angle(*module);
    add_interval(*module);
    add_easing(*module);

    return module;
}

#endif
