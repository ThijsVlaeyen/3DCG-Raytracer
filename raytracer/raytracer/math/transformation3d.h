#pragma once

#include "math/matrix.h"
#include "math/angle.h"

namespace math
{
    /// <summary>
    /// Represents a transformation in 3D. Keeps both the transformation matrix and its inverse.
    /// </summary>
    class Transformation3D
    {
    public:
        Transformation3D(const Matrix4x4& transformation_matrix, const Matrix4x4& inverse_transformation_matrix)
            : transformation_matrix(transformation_matrix), inverse_transformation_matrix(inverse_transformation_matrix)  { }
    
        Matrix4x4 transformation_matrix;
        Matrix4x4 inverse_transformation_matrix;
    };

    namespace transformations
    {
        Transformation3D translation(const Vector3D&);
        Transformation3D rotate_y(Angle);
        Transformation3D rotate_x(Angle);
    }
}
