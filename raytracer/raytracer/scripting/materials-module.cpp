#ifndef EXCLUDE_SCRIPTING

#include "scripting/materials-module.h"
#include "scripting/scripting-util.h"
#include "materials/materials.h"
#include "imaging/color.h"

using namespace chaiscript;
using namespace raytracer;
using namespace math;
using namespace imaging;


namespace
{
    /// <summary>
    /// Material Library. Groups all material-related functions together.
    /// </summary>
    struct MaterialLibrary
    {
        Material uniform(const Color& ambient, const Color& diffuse, const Color& specular, double specular_exponent, double reflectivity, double transparency, double refractive_index) const
        {
            MaterialProperties properties(ambient, diffuse, specular, specular_exponent, reflectivity, transparency, refractive_index);

            return raytracer::materials::uniform(properties);
        }

        Material uniform_by_map(const std::map<std::string, Boxed_Value>& argument_map) const
        {
            START_ARGUMENTS(argument_map);
            OPTIONAL_ARGUMENT(Color, ambient, colors::black());
            OPTIONAL_ARGUMENT(Color, diffuse, colors::black());
            OPTIONAL_ARGUMENT(Color, specular, colors::black());
            OPTIONAL_ARGUMENT(double, specular_exponent, 10);
            OPTIONAL_ARGUMENT(double, reflectivity, 0.0);
            OPTIONAL_ARGUMENT(double, transparency, 0.0);
            OPTIONAL_ARGUMENT(double, refractive_index, 1.0);
            END_ARGUMENTS();

            MaterialProperties properties(ambient, diffuse, specular, specular_exponent, reflectivity, transparency, refractive_index);

            return raytracer::materials::uniform(properties);
        }

        Material horizontal_lines(double thickness, Material m1, Material m2) const
        {
            return raytracer::materials::horizontal_lines(thickness, m1, m2);
        }

        Material vertical_lines(double thickness, Material m1, Material m2) const
        {
            return raytracer::materials::vertical_lines(thickness, m1, m2);
        }
    };
}

ModulePtr raytracer::scripting::_private_::create_materials_module()
{
    // Create new module
    auto module = std::make_shared<chaiscript::Module>();

    // Tell chaiscript about Material type
    util::register_type<Material>(*module, "Material");

    // Create library
    auto material_library = std::make_shared<MaterialLibrary>();

    // Expose library (member functions still need to be exposed separately)
    module->add_global_const(chaiscript::const_var(material_library), "Materials");

    // Exposes library function named INTERNAL as chaiscript function named EXTERNAL
#   define BIND_AS(INTERNAL, EXTERNAL)     module->add(fun(&MaterialLibrary::INTERNAL), #EXTERNAL)

    // Exposes library function under the same name
#   define BIND(NAME)                      BIND_AS(NAME, NAME)
    BIND(uniform);
    BIND_AS(uniform_by_map, uniform);
    BIND(horizontal_lines);
    BIND(vertical_lines);
#   undef BIND
#   undef BIND_AS

    return module;
}

#endif
