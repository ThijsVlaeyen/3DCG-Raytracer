#ifndef EXCLUDE_SCRIPTING

#include "scripting/raytracing-module.h"
#include "raytracers/ray-tracers.h"
#include "scripting/scripting-util.h"


using namespace chaiscript;
using namespace raytracer;
using namespace raytracer::scripting;


namespace
{
    struct RaytracerLibrary
    {
        RayTracer v0() const { return raytracer::raytracers::v0(); }
        RayTracer v1() const { return raytracer::raytracers::v1(); }
        RayTracer v2() const { return raytracer::raytracers::v2(); }
        RayTracer v3() const { return raytracer::raytracers::v3(); }

        RayTracer v(int version) const
        {
            switch (version)
            {
#           define DISPATCH(N) case N: return v ## N()
                DISPATCH(0);
                DISPATCH(1);
                DISPATCH(2);
                DISPATCH(3);
#           undef DISPATCH

            default:
                LOG(ERROR) << "Unknown ray tracer version " << version;
                abort();
            }
        }
    };

    std::shared_ptr<Scene> create_scene(Camera camera, Primitive root, const std::vector<Boxed_Value>& boxed_lights)
    {
        auto unboxed_lights = util::cast_vector_elements<LightSource>(boxed_lights);
        return std::make_shared<Scene>(camera, root, unboxed_lights);
    }
}

ModulePtr raytracer::scripting::_private_::create_raytracing_module()
{
    // Create new chaiscript module
    auto module = std::make_shared<chaiscript::Module>();

    // Tell chaiscript about the RayTracer type
    raytracer::scripting::util::register_type<raytracer::RayTracer>(*module, "RayTracer");

    // Create library
    auto raytracer_library = std::make_shared<RaytracerLibrary>();

    // Expose library under the given name (member functions need to be exposed separately)
    module->add_global_const(chaiscript::const_var(raytracer_library), "Raytracers");

    // Expose each member of the library
#   define BIND(NAME)   module->add(fun(&RaytracerLibrary::NAME), #NAME)
        BIND(v0);
        BIND(v1);
        BIND(v2);
        BIND(v3);
        BIND(v);
#   undef BIND

    // Expose create_scene under the same name
    module->add(fun(&create_scene), "create_scene");

    return module;
}

#endif
