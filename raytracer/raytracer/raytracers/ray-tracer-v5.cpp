#include "raytracers/ray-tracer-v5.h"
#include "raytracers/ray-tracer.h"

using namespace imaging;
using namespace math;
using namespace raytracer;


TraceResult raytracer::raytracers::_private_::RayTracerV5::trace(const Scene& scene, const Ray& ray) const
{
    Hit hit;

    if (scene.root->find_first_positive_hit(ray, &hit))
    {
        unsigned group_id = hit.group_id;
        double t = hit.t;
        // New return with the new trace and weight
        return TraceResult(trace(scene, ray, 1.0), group_id, ray, t);
    }
    else
    {
        return TraceResult::no_hit(ray);
    }
}

raytracer::RayTracer raytracer::raytracers::v5()
{
    return raytracer::RayTracer(std::make_shared<raytracer::raytracers::_private_::RayTracerV5>());
}

imaging::Color raytracer::raytracers::_private_::RayTracerV5::trace(const Scene& scene, const math::Ray& ray, double weight) const
{
	// Just return black if weight is too small
	Color result = colors::black();

	// Checking the weight for the infinit recursions!
	if (weight > 0.01)
	{
		Hit hit;

		// Same as before
		if (scene.root->find_first_positive_hit(ray, &hit))
		{
			double t = hit.t;
			MaterialProperties props = hit.material->at(hit.local_position);

			result += compute_ambient(props);
			result += process_lights(scene, props, hit, ray);

			// The new and fance reflection (foreshadowing this will increase rendertimes rip pc)
			result += compute_reflection(scene, props, ray, hit, weight);
		}
	}

	return result;
}

imaging::Color raytracer::raytracers::_private_::RayTracerV5::compute_reflection(const Scene& scene, const MaterialProperties& props, const math::Ray& ray, const Hit& reflect, double weight) const
{
	// Just return black if material is not reflective
	Color result = colors::black();

	if (props.reflectivity > 0) 
	{
		// Throw in some formulas 
		// For helping me understand what the formulas do credits to the one and only Michiel Vlaeyen aka de Chelle

		Vector3D direction_ray = (reflect.position - ray.origin).normalized();
		Vector3D direction_reflection = direction_ray.reflect_by(reflect.normal);
		// 0.00000001 is to move the origin a small amount
		Point3D new_Origin = ray.at(reflect.t) + 0.00000001 * direction_reflection;
		Ray reflection = Ray(new_Origin, direction_reflection);

		// I see you in the next recursion
		result += (props.reflectivity * trace(scene, reflection, props.reflectivity * weight));
	}

	return result;
}