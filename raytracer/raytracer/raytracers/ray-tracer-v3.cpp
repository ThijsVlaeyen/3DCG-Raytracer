#include "raytracers/ray-tracer-v3.h"
#include "raytracers/ray-tracer.h"

using namespace imaging;
using namespace math;
using namespace raytracer;

raytracer::RayTracer raytracer::raytracers::v3()
{
    return raytracer::RayTracer(std::make_shared<raytracer::raytracers::_private_::RayTracerV3>());
}

Color raytracer::raytracers::_private_::RayTracerV3::process_light_ray(const Scene& scene, const MaterialProperties& props, const Hit& hit, const math::Ray& ray, const LightRay& lightray) const 
{
	Color result = RayTracerV2::process_light_ray(scene, props, hit, ray, lightray);
	result += compute_specular(props, hit, ray, lightray);
	return result;
}

Color raytracer::raytracers::_private_::RayTracerV3::compute_specular(const MaterialProperties& props, const Hit& hit, const math::Ray& ray, const LightRay& lightray) const
{
	Color result = colors::black();

	// Big brain math time
	// Compute the direction of the incoming light.
	Vector3D direction_Incoming = (hit.position - lightray.ray.origin).normalized();

	// Compute the reflected direction.
	Vector3D direction_Reflected = direction_Incoming.reflect_by(hit.normal);

	// Compute the unit vector going from the hit P to E.
	Vector3D unit_Vector = (ray.origin - hit.position).normalized();

	// Compute the cosine of the angle between r-> and v->.
	double cos_Alpha = unit_Vector.dot(direction_Reflected);

	// If cos α > 0, the amount of photons reaching E from L is CL ⋅ CP	⋅ cos(α)	e
	if (cos_Alpha > 0)
	{
		result += (lightray.color * props.specular * pow(cos_Alpha, props.specular_exponent));
	}

	return result;
}

