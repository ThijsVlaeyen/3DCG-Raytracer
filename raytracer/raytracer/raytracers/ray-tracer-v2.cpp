#include "raytracers/ray-tracer-v2.h"
#include "raytracers/ray-tracer.h"

using namespace imaging;
using namespace math;
using namespace raytracer;


TraceResult raytracer::raytracers::_private_::RayTracerV2::trace(const Scene& scene, const Ray& ray) const
{
    Hit hit;

    // Ask the scene for the first positive hit, i.e. the closest hit in front of the eye
    // If there's a hit, find_first_positive_hit returns true and updates the hit object with information about the hit
    if (scene.root->find_first_positive_hit(ray, &hit))
    {
        // There's been a hit
        // Fill in TraceResult object with information about the trace

        // New Code
        Color result = colors::black();
        MaterialProperties props = hit.material->at(hit.local_position);
        result += compute_ambient(props);
        result += process_lights(scene, props, hit, ray);

        // The hit object contains the group id, just copy it (group ids are important for edge detection)
        unsigned group_id = hit.group_id;

        // The t-value indicates where the ray/scene intersection took place.
        // You can use ray.at(t) to find the xyz-coordinates in space.
        double t = hit.t;

        // Group all this data into a TraceResult object.
        return TraceResult(result, group_id, ray, t);
    }
    else
    {
        // The ray missed all objects in the scene
        // Return a TraceResult object representing "no hit found"
        // which is basically the same as returning black
        return TraceResult::no_hit(ray);
    }
}

raytracer::RayTracer raytracer::raytracers::v2()
{
    return raytracer::RayTracer(std::make_shared<raytracer::raytracers::_private_::RayTracerV2>());
}

Color raytracer::raytracers::_private_::RayTracerV2::process_lights(const Scene& scene, const MaterialProperties& props, const Hit& hit, const math::Ray& ray) const
{
    Color result = colors::black();

    for each (LightSource l in scene.light_sources) {
        result += process_light_source(scene, props, hit, ray, l);
    }

    return result;
}

Color raytracer::raytracers::_private_::RayTracerV2::process_light_source(const Scene& scene, const MaterialProperties& props, const Hit& hit, const math::Ray& ray, LightSource light) const
{
    Color result = colors::black();

    for each (LightRay l in light->lightrays_to(hit.position)) {
        result += process_light_ray(scene, props, hit, ray, l);
    }

    return result;
}

Color raytracer::raytracers::_private_::RayTracerV2::process_light_ray(const Scene& scene, const MaterialProperties& props, const Hit& hit, const math::Ray& ray, const LightRay& lightray) const
{
    Color result = colors::black();
    result += compute_diffuse(props, hit, ray, lightray);
    return result;
}

Color raytracer::raytracers::_private_::RayTracerV2::compute_diffuse(const MaterialProperties& props, const Hit& hit, const Ray& ray, const LightRay& lightray) const
{
    Color result = colors::black();
    double cosAlpha = ((lightray.ray.origin - hit.position).normalized()).dot(hit.normal);

    if (cosAlpha > 0)
    {
        result += (cosAlpha * lightray.color * props.diffuse);
    }

    return result;
}