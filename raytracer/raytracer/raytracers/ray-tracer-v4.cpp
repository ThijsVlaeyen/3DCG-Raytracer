#include "raytracers/ray-tracer-v4.h"
#include "raytracers/ray-tracer.h"

using namespace imaging;
using namespace math;
using namespace raytracer;

raytracer::RayTracer raytracer::raytracers::v4()
{
	return raytracer::RayTracer(std::make_shared<raytracer::raytracers::_private_::RayTracerV4>());
}

Color raytracer::raytracers::_private_::RayTracerV4::process_light_ray(const Scene& scene, const MaterialProperties& props, const Hit& hit, const math::Ray& ray, const LightRay& lightray) const
{

	Color result = colors::black();
	Hit rayhit;

	if (scene.root->find_first_positive_hit(lightray.ray, &rayhit))
	{
		double t = rayhit.t;

		if ((0.00 <= t) && (t <= 0.99))
		{
			// Return black if 0 <= t < 1
			return result;
		}
	}
	// Else the normal stuff
	result += RayTracerV3::process_light_ray(scene, props, hit, ray, lightray);
	return result;
}

