#include "primitives/plane-primitive.h"
#include "math/interval.h"

using namespace raytracer;
using namespace raytracer::primitives;
using namespace math;


namespace
{
    /// <summary>
    /// Superclass for planes. Contains common logic.
    /// </summary>
    class CoordinatePlaneImplementation : public raytracer::primitives::_private_::PrimitiveImplementation
    {
    protected:
        const Vector3D m_normal;

        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="normal">
        /// Normal vector on plane. Needs to have unit length.
        /// </param>
        CoordinatePlaneImplementation(const Vector3D& normal)
            : m_normal(normal)
        {
            assert(normal.is_unit());
        }

        virtual void initialize_hit(Hit* hit, const Ray& ray, double t) const = 0;

    public:
        std::vector<std::shared_ptr<Hit>> find_all_hits(const math::Ray& ray) const override
        {
            std::vector<std::shared_ptr<Hit>> hits;

            // Compute denominator
            double denom = ray.direction.dot(m_normal);

            // If denominator == 0, there is no intersection (ray runs parallel to plane)
            if (denom != approx(0.0))
            {
                // Compute numerator
                double numer = -((ray.origin - Point3D(0, 0, 0)).dot(m_normal));

                // Compute t
                double t = numer / denom;

                // Create hit object
                auto hit = std::make_shared<Hit>();

                // shared_ptr<T>::get() returns the T* inside the shared pointer
                initialize_hit(hit.get(), ray, t);

                // Put hit in list
                hits.push_back(hit);
            }

            return hits;
        }
    };

    class PlaneXYImplementation : public CoordinatePlaneImplementation
    {
    public:
        PlaneXYImplementation()
            : CoordinatePlaneImplementation(Vector3D(0, 0, 1))
        {
            // NOP
        }

        math::Box bounding_box() const override
        {
            return Box(Interval<double>::infinite(), Interval<double>::infinite(), interval(-0.01, 0.01));
        }

    protected:
        void initialize_hit(Hit* hit, const Ray& ray, double t) const override
        {
            hit->t = t;
            hit->position = ray.at(hit->t);
            hit->local_position.xyz = hit->position;
            hit->local_position.uv = Point2D(hit->position.x(), hit->position.y());
            hit->normal = ray.origin.z() > 0 ? m_normal : -m_normal;
        }
    };

    class PlaneXZImplementation : public CoordinatePlaneImplementation
    {
    public:
        PlaneXZImplementation()
            : CoordinatePlaneImplementation(Vector3D(0, 1, 0))
        {
            // NOP
        }

        math::Box bounding_box() const override
        {
            return Box(Interval<double>::infinite(), interval(-0.01, 0.01), Interval<double>::infinite());
        }

    protected:
        void initialize_hit(Hit* hit, const Ray& ray, double t) const override
        {
            hit->t = t;
            hit->position = ray.at(hit->t);
            hit->local_position.xyz = hit->position;
            hit->local_position.uv = Point2D(hit->position.x(), hit->position.y());
            hit->normal = ray.origin.y() > 0 ? m_normal : -m_normal;
        }
    };

    class PlaneYZImplementation : public CoordinatePlaneImplementation
    {
    public:
        PlaneYZImplementation()
            : CoordinatePlaneImplementation(Vector3D(1, 0, 0))
        {
            // NOP
        }

        math::Box bounding_box() const override
        {
            return Box(interval(-0.01, 0.01), Interval<double>::infinite(), Interval<double>::infinite());
        }

    protected:
        void initialize_hit(Hit* hit, const Ray& ray, double t) const override
        {
            hit->t = t;
            hit->position = ray.at(hit->t);
            hit->local_position.xyz = hit->position;
            hit->local_position.uv = Point2D(hit->position.x(), hit->position.y());
            hit->normal = ray.origin.x() > 0 ? m_normal : -m_normal;
        }
    };
}

Primitive raytracer::primitives::xy_plane()
{
    return Primitive(std::make_shared<PlaneXYImplementation>());
}

Primitive raytracer::primitives::xz_plane()
{
    return Primitive(std::make_shared<PlaneXZImplementation>());
}

Primitive raytracer::primitives::yz_plane()
{
    return Primitive(std::make_shared<PlaneYZImplementation>());
}