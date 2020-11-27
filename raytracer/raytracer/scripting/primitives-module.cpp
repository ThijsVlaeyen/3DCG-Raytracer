#ifndef EXCLUDE_SCRIPTING

#include "scripting/primitives-module.h"
#include "scripting/scripting-util.h"
#include "primitives/primitives.h"
#include "math/functions.h"
#include "animation/time-stamp.h"
#include "primitives/intersect-primitive.h"

using namespace chaiscript;
using namespace raytracer;
using namespace math;


namespace
{
    Primitive make_union(const std::vector<chaiscript::Boxed_Value>& boxed_children)
    {
        std::vector<Primitive> children(boxed_children.size());

        std::transform(boxed_children.begin(), boxed_children.end(), children.begin(), [](chaiscript::Boxed_Value boxed) {
            return chaiscript::boxed_cast<Primitive>(boxed);
        });

        return primitives::make_union(children);
    }
}

ModulePtr raytracer::scripting::_private_::create_primitives_module()
{
    auto module = std::make_shared<chaiscript::Module>();

    util::register_type<Primitive>(*module, "Primitive");
    util::register_assignment<Primitive>(*module);

    // Binds helper function defined earlier in this file, exposing the function under the same name
#   define BIND_HELPER_FUNCTION(NAME)                  BIND_HELPER_FUNCTION_AS(NAME, NAME)

    // Binds helper function defined earlier in this file, exposing the function under a different name
#   define BIND_HELPER_FUNCTION_AS(FACTORY, NAME)      module->add(fun(&FACTORY), #NAME)

    // Bypasses helper functions and directly binds to a function from raytracer::primitives
#   define BIND_DIRECTLY(NAME)                         BIND_HELPER_FUNCTION_AS(raytracer::primitives::NAME, NAME)
    BIND_DIRECTLY(sphere);
    BIND_DIRECTLY(triangle);
    BIND_DIRECTLY(xy_plane);
    BIND_DIRECTLY(xz_plane);
    BIND_DIRECTLY(yz_plane);
    BIND_DIRECTLY(xy_square);
    BIND_DIRECTLY(xz_square);
    BIND_DIRECTLY(yz_square);
    BIND_DIRECTLY(cylinder_along_y);
    BIND_DIRECTLY(cone_along_y);
    BIND_DIRECTLY(make_intersect);
    BIND_DIRECTLY(make_difference);
    BIND_HELPER_FUNCTION_AS(make_union, union);
    BIND_DIRECTLY(decorate);
    BIND_DIRECTLY(translate);
    BIND_DIRECTLY(rotate_around_y);
    BIND_DIRECTLY(rotate_around_x);
    BIND_DIRECTLY(rotate_around_z);
    BIND_DIRECTLY(scale);
    BIND_DIRECTLY(cube);
#   undef BIND_HELPER_FUNCTION_AS
#   undef BIND_DIRECTLY
#   undef BIND_HELPER_FUNCTION

    return module;
}

#endif
