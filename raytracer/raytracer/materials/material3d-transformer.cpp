#include "materials/material3d-transformer.h"
#include "materials/materials.h"
#include "materials/material.h"
#include "math/transformation3d.h"
using namespace math;
using namespace raytracer;
using namespace raytracer::materials;

class Material3DTransformer : public raytracer::materials::_private_::MaterialImplementation
{
public:
	Material3DTransformer(const math::Transformation3D transformer, Material transformee)
		: transformer(transformer), transformee(transformee) { }



	MaterialProperties at(const HitPosition& p) const override
	{
		auto hit = HitPosition(p);
		hit.xyz = transformer.inverse_transformation_matrix * p.xyz;
		return transformee->at(hit);
	}

private:
	math::Transformation3D transformer;
	Material transformee;
};

Material materials::translation3d(Vector3D& s, Material m) //VS translation3D
{
	return Material(std::make_shared<Material3DTransformer>(Transformation3D(math::transformations::translation(s)), m));
}