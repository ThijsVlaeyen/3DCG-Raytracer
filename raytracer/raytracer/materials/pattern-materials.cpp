#include "materials/material.h"
#include "materials/composition-material.h"
#include "materials/pattern-materials.h"
#include "math/function.h"
#include "math/functions.h"
#include "math/functions/patterns.h"
#include "math/functions/noise.h"
#include "math/functions/bool-mapper.h"
#include "imaging/color-mapper.h"

using namespace math;
using namespace raytracer;
using namespace raytracer::materials;
using namespace imaging;


Material raytracer::materials::pattern2d(math::Function<bool(const Point2D&)> pattern, Material m1, Material m2)
{
    auto bool_mapper = math::functions::bool_mapper(m1, m2);

    return composite(pattern >> bool_mapper);
}

Material raytracer::materials::horizontal_lines(double thickness, Material m1, Material m2)
{
    return pattern2d(math::functions::horizontal_lines(thickness), m1, m2);
}

Material raytracer::materials::vertical_lines(double thickness, Material m1, Material m2)
{
    return pattern2d(math::functions::vertical_lines(thickness), m1, m2);
}

Material raytracer::materials::grid2d(double thickness, Material m1, Material m2)
{
    return pattern2d(math::functions::grid2d(thickness), m1, m2);
}