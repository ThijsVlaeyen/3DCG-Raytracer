#include "samplers/multi-jittered-sampler.h"
#include "math/rasterizer.h"
#include <numeric>

using namespace math;
using namespace raytracer;

namespace
{
	class MultiJitteredSampler : public samplers::_private_::SamplerImplementation
	{
	public:
		int n;

		explicit MultiJitteredSampler(const int& n)
		{
			this->n = n;
		}

		std::vector<Point2D> sample(const Rectangle2D& rectangle) const override
		{
			std::vector<Point2D> result;
			auto rasterizer = Rasterizer(rectangle, n, n);
			std::vector<int> cols(n*n);
			std::iota(std::begin(cols), std::end(cols), 0);

			for (auto i = 0; i < n*n; i++)
			{
				
				const auto col = rand() % cols.size();
				cols.erase(cols.begin() + col);
				auto sub_rectangle = rasterizer[Position2D(i / n, i % n)];
				auto sub_rasterizer = Rasterizer(sub_rectangle, n, n);
				auto sub_sub_rectangle = sub_rasterizer[Position2D(col / n, col % n)];
				const auto x = find_rand(0, 1);
				const auto y = find_rand(0, 1);
				result.push_back(sub_sub_rectangle.from_relative(Point2D(x, y)));
			}
			return result;
		}

		static double find_rand(const double minimum, const double maximum)
		{
			const auto a = static_cast<double>(rand()) / RAND_MAX;
			return minimum + a * (maximum - minimum);
		}
	};
}

Sampler samplers::multi_jittered(const int& n)
{
	return Sampler(std::make_shared<MultiJitteredSampler>(n));
}