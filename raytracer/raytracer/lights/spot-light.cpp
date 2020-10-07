#include "lights/spot-light.h"

using namespace math;
using namespace raytracer;

namespace
{
	/// <summary>
	/// Implementation for spot lights.
	/// </summary>
	class SpotLight : public raytracer::lights::_private_::PointLightImplementation
	{
	public:
		//Constructor with Position L, Central shining direction v and beam angle θ
		SpotLight(const Point3D& position, const Vector3D& direction, const Angle& angle, const imaging::Color& color)
			: PointLightImplementation(position), m_direction(direction), m_angle(angle), m_color(color) {}

	protected:
		LightRay cast_lightray_to(const Point3D& position) const override
		{
			// Formula: http://3dcg.leone.ucll.be/reference/lights/spot/explanations.html
			Ray ray(m_position, position);
			// Part left side
			Vector3D u = (position - m_position).normalized();
			// Right side
			double cosine = cos(m_angle / 2);
			if (u.dot(m_direction) >= cosine)
			{
				// Return colour
				return LightRay(ray, m_color);
			}
			// Return black
			return LightRay(ray, imaging::colors::black());
		}

	private:
		Vector3D m_direction;
		Angle m_angle;
		imaging::Color m_color;
	};
}

LightSource raytracer::lights::spot(const Point3D& position, const Vector3D& direction, const Angle& angle, const imaging::Color& color)
{
	return LightSource(std::make_shared<SpotLight>(position, direction, angle, color));
}
