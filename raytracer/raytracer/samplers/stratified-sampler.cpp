#include "samplers/stratified-sampler.h"
#include "math/rasterizer.h"

using namespace math;
using namespace raytracer;

namespace
{
	class StratifiedSampler : public samplers::_private_::SamplerImplementation
	{
	public:
		int rows;
		int cols;

		explicit StratifiedSampler(const int& rows, const int& cols)
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
					result.push_back(sub_rectangle.center());
				}
			}
			return result;
		}
	};
}

Sampler samplers::stratified(const int& rows, const int& cols)
{
	return Sampler(std::make_shared<StratifiedSampler>(rows, cols));
}