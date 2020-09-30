#include "ray-tracer-v6.h"
#include "ray-tracer-v5.h"
#include "raytracers/ray-tracers.h"

using namespace imaging;
using namespace math;
using namespace raytracer;


RayTracer raytracer::raytracers::v6()
{
	return raytracer::RayTracer(std::make_shared<raytracer::raytracers::_private_::RayTracerV6>());
}

imaging::Color raytracer::raytracers::_private_::RayTracerV6::trace(const Scene& scene, const math::Ray& ray, double weight) const
{
	Color result = colors::black();

	if (weight > 0.01)
	{
		Hit hit;

		if (scene.root->find_first_positive_hit(ray, &hit))
		{
			MaterialProperties props = hit.material->at(hit.local_position);

			result += compute_ambient(props);
			result += process_lights(scene, props, hit, ray);
			result += compute_reflection(scene, props, ray, hit, weight);
			result += compute_refraction(scene, props, ray, hit, weight);

			return result;
		}
	}
	return result;
}

imaging::Color raytracer::raytracers::_private_::RayTracerV6::compute_refraction(const Scene& scene, const MaterialProperties& props, const math::Ray& ray, const Hit& hit, double weight) const
{
	if (props.transparency > 0)
	{
		Ray refracted_ray = compute_refracted_ray(scene, ray, hit, 1.0, props.refractive_index);

		if (refracted_ray == ray)
		{
			return colors::black();
		}

		// Check if there is an exit otherwise skip and return black
		Hit exit_hit;
		if (scene.root->find_first_positive_hit(refracted_ray, &exit_hit))
		{
			Ray exit_ray = compute_refracted_ray(scene, refracted_ray, exit_hit, props.refractive_index, 1.0);
			// BEAM ME UP SCOTTY
			return trace(scene, exit_ray, weight * props.transparency) * props.transparency;
		}
	}
	return colors::black();
}

math::Ray raytracer::raytracers::_private_::RayTracerV6::compute_refracted_ray(const Scene& scene, const math::Ray& ray, const Hit& hit, double n1, double n2) const
{
	// FOR MORE MATH AND FUN CHECKOUT THIS PAGE http://3dcg.leone.ucll.be/reference/refraction/explanations.html
	Vector3D direction_incoming = (hit.position - ray.origin).normalized();

	Vector3D outgoing_x = (n1 / n2) * (direction_incoming - (direction_incoming.dot(hit.normal)) * hit.normal);

	if ((1 - outgoing_x.norm_sqr()) < 0)
	{
		return ray;
	}

	Vector3D outgoing_y = (0 - sqrt(1 - outgoing_x.norm_sqr())) * hit.normal;
	Vector3D direction_outgoing = outgoing_x + outgoing_y;
	Point3D outgoing_origin = ray.at(hit.t) + (0.00000001 * direction_outgoing);
	Ray refracted_ray = Ray(outgoing_origin, direction_outgoing);

	return refracted_ray;
}


