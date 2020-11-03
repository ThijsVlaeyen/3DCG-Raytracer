#include "samplers/single-sampler.h"
#include "random-sampler.h"
#include <random>

using namespace math;
using namespace raytracer;

namespace
{
	class RandomSampler : public raytracer::samplers::_private_::SamplerImplementation
	{
	private:
		int count;
	public:
		RandomSampler(int count) : count(count){}

		std::vector<Point2D> sample(const math::Rectangle2D& rectangle) const override
		{
			std::vector<Point2D> result;

			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<double> dis(0, 1);

			for (int i = 0; i < count; i++) {
				double x = dis(gen);
				double y = dis(gen);
				result.push_back(rectangle.from_relative(Point2D(x, y)));
			}
			return result;
		}
	};
}

Sampler raytracer::samplers::random(int count)
{
	return Sampler(std::make_shared<RandomSampler>(count));
}