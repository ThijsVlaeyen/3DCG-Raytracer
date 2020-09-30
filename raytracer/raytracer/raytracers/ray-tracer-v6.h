#pragma once

#include "raytracers/ray-tracer-v5.h"
#include <memory>

namespace raytracer
{
	namespace raytracers
	{
		namespace _private_
		{
			class RayTracerV6 : public RayTracerV5
			{
			protected:
				imaging::Color trace(const Scene&, const math::Ray&, double) const override;
				imaging::Color compute_refraction(const Scene&, const MaterialProperties&, const math::Ray&, const Hit&, double) const;
				math::Ray compute_refracted_ray(const Scene&, const math::Ray&, const Hit&, double, double) const;
			};
		}
		RayTracer v6();
	}
}