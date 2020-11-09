#ifndef EXCLUDE_SCRIPTING

#include "scripting/samplers-module.h"
#include "samplers/samplers.h"
#include "scripting/scripting-util.h"

using namespace chaiscript;
using namespace raytracer;
using namespace math;


namespace
{
    struct SamplerLibrary
    {
        Sampler single() const
        {
            return samplers::single();
        }
        Sampler random(int count) const
        {
            return samplers::random(count);
        }
        Sampler stratified(const int& rows, const int& cols) const
        {
            return samplers::stratified(rows, cols);
        }
        Sampler jittered(const int& rows, const int& cols) const
        {
            return samplers::jittered(rows, cols);
        }
        Sampler half_jittered(const int& rows, const int& cols) const
        {
            return samplers::half_jittered(rows, cols);
        }
    };
}

ModulePtr raytracer::scripting::_private_::create_samplers_module()
{
    auto module = std::make_shared<chaiscript::Module>();

    util::register_type<Sampler>(*module, "Sampler");

    auto sampler_library = std::make_shared<SamplerLibrary>();
    module->add_global_const(chaiscript::const_var(sampler_library), "Samplers");

#   define BIND(NAME)  module->add(fun(&SamplerLibrary::NAME), #NAME)
    BIND(single);
    BIND(random);
    BIND(stratified);
    BIND(jittered);
    BIND(half_jittered);
#   undef BIND

    return module;
}

#endif
