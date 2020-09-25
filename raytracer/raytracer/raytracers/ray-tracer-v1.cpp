#include "raytracers/ray-tracer-v1.h"
#include "raytracers/ray-tracer.h"

using namespace imaging;
using namespace math;
using namespace raytracer;


TraceResult raytracer::raytracers::_private_::RayTracerV1::trace(const Scene& scene, const Ray& ray) const
{
    Hit hit;

    // Ask the scene for the first positive hit, i.e. the closest hit in front of the eye
    // If there's a hit, find_first_positive_hit returns true and updates the hit object with information about the hit
    if (scene.root->find_first_positive_hit(ray, &hit))
    {
        // There's been a hit
        // Fill in TraceResult object with information about the trace

        // The hit object contains the group id, just copy it (group ids are important for edge detection)
        unsigned group_id = hit.group_id;

        // The t-value indicates where the ray/scene intersection took place.
        // You can use ray.at(t) to find the xyz-coordinates in space.
        double t = hit.t;

        // Group all this data into a TraceResult object.
        return TraceResult(compute_ambient(hit.material->at(hit.local_position)), group_id, ray, t);
    }
    else
    {
        // The ray missed all objects in the scene
        // Return a TraceResult object representing "no hit found"
        // which is basically the same as returning black
        return TraceResult::no_hit(ray);
    }
}

raytracer::RayTracer raytracer::raytracers::v1()
{
    return raytracer::RayTracer(std::make_shared<raytracer::raytracers::_private_::RayTracerV1>());
}

Color raytracer::raytracers::_private_::RayTracerV1::compute_ambient(const MaterialProperties& props) const
{
    return props.ambient;
}