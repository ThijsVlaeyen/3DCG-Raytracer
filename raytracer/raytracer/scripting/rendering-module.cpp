#ifndef EXCLUDE_SCRIPTING

#include "scripting/rendering-module.h"
#include "scripting/scripting-util.h"
#include "renderers/renderers.h"
#include "raytracers/ray-tracer.h"
#include "samplers/samplers.h"
#include "imaging/wif-format.h"
#include "imaging/ppm-format.h"
#include "tasks/task-schedulers.h"

using namespace chaiscript;
using namespace raytracer;


namespace
{
    struct RendererLibrary
    {
        Renderer standard(unsigned width, unsigned height, Sampler sampler, RayTracer ray_tracer) const
        {
            return raytracer::renderers::standard(width, height, sampler, ray_tracer, tasks::schedulers::serial());
        }

        Renderer standard_by_map(const std::map<std::string, Boxed_Value>& argument_map) const
        {
            START_ARGUMENTS(argument_map);
            ARGUMENT(unsigned, width);
            ARGUMENT(unsigned, height);
            ARGUMENT(Sampler, sampler);
            ARGUMENT(RayTracer, ray_tracer);
            END_ARGUMENTS();

            return standard(width, height, sampler, ray_tracer);
        }

        Renderer parallel(unsigned width, unsigned height, Sampler sampler, RayTracer ray_tracer) const
        {
            return raytracer::renderers::standard(width, height, sampler, ray_tracer, tasks::schedulers::parallel());
        }

        Renderer parallel_by_map(const std::map<std::string, Boxed_Value>& argument_map) const
        {
            START_ARGUMENTS(argument_map);
            ARGUMENT(unsigned, width);
            ARGUMENT(unsigned, height);
            ARGUMENT(Sampler, sampler);
            ARGUMENT(RayTracer, ray_tracer);
            END_ARGUMENTS();

            return parallel(width, height, sampler, ray_tracer);
        }
    };
}

ModulePtr raytracer::scripting::_private_::create_rendering_module()
{
    auto module = std::make_shared<chaiscript::Module>();

    util::register_type<Renderer>(*module, "Renderer");

    auto renderer_library = std::make_shared<RendererLibrary>();
    module->add_global_const(chaiscript::const_var(renderer_library), "Renderers");

#   define BIND_AS(INTERNAL, EXTERNAL) module->add(fun(&RendererLibrary::INTERNAL), #EXTERNAL)
    BIND_AS(standard, standard);
    BIND_AS(standard_by_map, standard);
    BIND_AS(parallel, parallel);
    BIND_AS(parallel_by_map, parallel);
#   undef BIND_AS

    return module;
}

#endif
