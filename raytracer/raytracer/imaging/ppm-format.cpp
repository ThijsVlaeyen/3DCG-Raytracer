#include "imaging/ppm-format.h"
#include <bitset>

using namespace imaging;

namespace 
{
	struct RGBColor
	{
		double r, g, b;
		RGBColor(const Color& c) {
			Color clamped = c.clamped();

			r = floor(clamped.r * 255);
			g = floor(clamped.g * 255);
			b = floor(clamped.b * 255);
		}

	};
}

namespace imaging
{
	void write_text_ppm(const Bitmap& bitmap, std::ostream& out)
	{
		int height = bitmap.height();
		int width = bitmap.width();

		//PPM HEADER
		out << "P3\n"
			<< width << " " << height << "\n"
			<< "255\n";

		//PPM PAYLOAD
		for (unsigned i = 0; i != height; i++)
		{
			for (unsigned j = 0; j != width; j++)
			{
				Color color(bitmap[Position2D(j, i)]);
				RGBColor rgb(color);
				out << rgb.r << " " << rgb.g << " " << rgb.b << "      ";
			}
			out << "\n";
		}
	}
}