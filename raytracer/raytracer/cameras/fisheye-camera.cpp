#include "cameras/perspective-camera.h"
#include "math/rectangle3d.h"
#include <assert.h>

using namespace raytracer;
using namespace raytracer::cameras;
using namespace math;


raytracer::cameras::_private_::PerspectiveCamera::PerspectiveCamera(const math::Matrix4x4 transformation, const Rectangle3D& view_window)
    : DisplaceableCamera(transformation), m_view_window(view_window)
{
    // NOP
}

void raytracer::cameras::_private_::PerspectiveCamera::enumerate_untransformed_rays(const Point2D& point, std::function<void(const math::Ray&)> callback) const
{
    assert(interval(0.0, 1.0).contains(point.x()));
    assert(interval(0.0, 1.0).contains(point.y()));

    Point3D p = m_view_window.project(point);

    callback(Ray(math::Point3D(0, 0, 0), p));
}

Camera raytracer::cameras::perspective(
    const math::Point3D& eye,
    const math::Point3D& look_at,
    const math::Vector3D& up,
    double distance,
    double aspect_ratio)
{
    assert(up.is_unit());

    double view_window_width = aspect_ratio;
    double view_window_height = 1;
    Point3D view_window_origin = Point3D(view_window_width / 2, -view_window_height / 2, distance);
    Vector3D view_window_right = Vector3D(-view_window_width, 0, 0);
    Vector3D view_window_up = Vector3D(0, view_window_height, 0);
    Rectangle3D view_window(view_window_origin, view_window_right, view_window_up);
    Matrix4x4 transformation = _private_::create_transformation(eye, look_at, up);

    return Camera(std::make_shared<_private_::PerspectiveCamera>(transformation, view_window));
}