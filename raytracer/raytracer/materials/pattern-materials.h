#pragma once

#include "materials/material.h"
#include "math/function.h"
#include "math/point.h"

namespace raytracer
{
    namespace materials
    {
        Material pattern2d(math::Function<bool(const math::Point2D&)>, Material, Material);
        Material pattern3d(math::Function<bool(const math::Point3D&)>, Material, Material);
        Material horizontal_lines(double, Material, Material);
        Material vertical_lines(double, Material, Material);
    }
}