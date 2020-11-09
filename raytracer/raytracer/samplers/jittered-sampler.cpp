#include "samplers/jittered-sampler.h"
#include "math/rasterizer.h"

using namespace math;
using namespace raytracer;

namespace
{
	class JitteredSampler : public samplers::_private_::SamplerImplementation
	{
	public:
		int rows;
		int cols;

		explicit JitteredSampler(const int& rows, const int& cols)
		{
			this->rows = rows;
			this->cols = cols;
		}

		std::vector<Point2D> sample(const Rectangle2D& rectangle) const override
		{
			std::vector<Point2D> result;
			auto rasterizer = Rasterizer(rectangle, rows, cols);

			for (auto i = 0; i < rows; i++)
			{
				for (auto j = 0; j < cols; j++)
				{
					auto sub_rectangle = rasterizer[Position2D(i, j)];
					const auto x = find_rand(0, 1);
					const auto y = find_rand(0, 1);
					result.push_back(sub_rectangle.from_relative(Point2D(x, y)));
				}
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

Sampler samplers::jittered(const int& rows, const int& cols)
{
	return Sampler(std::make_shared<JitteredSampler>(rows, cols));
}