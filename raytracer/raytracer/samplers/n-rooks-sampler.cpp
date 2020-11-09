#include "samplers/n-rooks-sampler.h"
#include "math/rasterizer.h"
#include <numeric>

using namespace math;
using namespace raytracer;

namespace
{
	class NRooksSampler : public samplers::_private_::SamplerImplementation
	{
	public:
		int rooks;

		explicit NRooksSampler(const int& rooks)
		{
			this->rooks = rooks;
		}

		std::vector<Point2D> sample(const Rectangle2D& rectangle) const override
		{
			std::vector<Point2D> result;
			auto rasterizer = Rasterizer(rectangle, rooks, rooks);
			std::vector<int> cols(rooks);
			std::iota(std::begin(cols), std::end(cols), 0);
			for (auto i = 0; i < rooks; i++)
			{
					const auto col = rand() % cols.size();
					cols.erase(cols.begin() + col);
					auto sub_rectangle = rasterizer[Position2D(i, cols[col])];
					const auto x = find_rand(0, 1);
					const auto y = find_rand(0, 1);
					result.push_back(sub_rectangle.from_relative(Point2D(x, y)));
				
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

Sampler samplers::n_rooks(const int& rooks)
{
	return Sampler(std::make_shared<NRooksSampler>(rooks));
}