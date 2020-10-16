#include "materials/material2d-transformer.h"
#include "materials/materials.h"
#include "materials/material.h"
#include "math/transformation2d.h"

using namespace math;
using namespace raytracer;
using namespace raytracer::materials;


class Material2DTransformer : public raytracer::materials::_private_::MaterialImplementation
{
	public:
		Material2DTransformer(const math::Transformation2D transformer, Material transformee) : transformer(transformer), transformee(transformee){}


		MaterialProperties at(const HitPosition& p) const override
		{
			auto hit = HitPosition(p);
			hit.uv = transformer.inverse_transformation_matrix * p.uv;
			return transformee->at(hit);
		}

	private: 
		math::Transformation2D transformer;
		Material transformee;

};

Material materials::translation(math::Vector2D& s, Material m)
{
	return Material(std::make_shared<Material2DTransformer>(Transformation2D(math::transformations::translation(s)), m));
};