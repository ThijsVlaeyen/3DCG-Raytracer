#include "math/transformation3d.h"
#include "math/transformation-matrices.h"

using namespace math;


Transformation3D math::transformations::translation(const Vector3D& v)
{
    Matrix4x4 tm = transformation_matrices::translation(v);
    Matrix4x4 itm = transformation_matrices::translation(-v);

    return Transformation3D(tm, itm);
}

Transformation3D math::transformations::rotate_y(Angle a)

{
	Matrix4x4 tm = transformation_matrices::rotation_around_y(a);
	Matrix4x4 itm = transformation_matrices::rotation_around_y(-a);

	return Transformation3D(tm, itm);
}

Transformation3D math::transformations::rotate_x(Angle a)
{
	Matrix4x4 tm = transformation_matrices::rotation_around_x(a);
	Matrix4x4 itm = transformation_matrices::rotation_around_x(-a);
	return Transformation3D(tm, itm);
}

Transformation3D math::transformations::rotate_z(Angle a)
{
	Matrix4x4 tm = transformation_matrices::rotation_around_z(a);
	Matrix4x4 itm = transformation_matrices::rotation_around_z(-a);
	return Transformation3D(tm, itm);
}

Transformation3D math::transformations::scale(const double sx, const double sy, const double sz)
{
	const auto tm = transformation_matrices::scaling(sx, sy, sz);
	const auto itm = transformation_matrices::scaling(1 / sx, 1 / sy, 1 / sz);

	return Transformation3D(tm, itm);
}
