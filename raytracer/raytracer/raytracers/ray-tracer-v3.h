#pragma once

#include "raytracers/ray-tracer-v2.h"
#include <memory>


namespace raytracer
{
    namespace raytracers
    {
        namespace _private_
        {
            class RayTracerV3 : public RayTracerV2
            {
            protected:
                virtual imaging::Color process_light_ray(const Scene&, const MaterialProperties&, const Hit&, const math::Ray&, const LightRay&) const override;
                imaging::Color compute_specular(const MaterialProperties&, const Hit&, const math::Ray&, const LightRay&) const;
            };
        }

        /// <summary>
        /// Creates new and improved ray tracer mk3.
        /// </summary>
        RayTracer v3();
    }
}