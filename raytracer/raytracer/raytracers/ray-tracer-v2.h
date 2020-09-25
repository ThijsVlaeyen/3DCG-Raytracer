#pragma once

#include "raytracers/ray-tracer-v1.h"
#include <memory>


namespace raytracer
{
    namespace raytracers
    {
        namespace _private_
        {
            class RayTracerV2 : public RayTracerV1
            {
            public:
                TraceResult trace(const Scene&, const math::Ray&) const override;
            protected:
                imaging::Color process_lights(const Scene&, const MaterialProperties&, const Hit&, const math::Ray&) const;
                imaging::Color process_light_source(const Scene&, const MaterialProperties&, const Hit&, const math::Ray&, LightSource) const;
                virtual imaging::Color process_light_ray(const Scene&, const MaterialProperties&, const Hit&, const math::Ray&, const LightRay&) const;
                imaging::Color compute_diffuse(const MaterialProperties&, const Hit&, const math::Ray&, const LightRay&) const;
            };
        }

        /// <summary>
        /// Creates new and improved ray tracer mk1.
        /// </summary>
        RayTracer v2();
    }
}