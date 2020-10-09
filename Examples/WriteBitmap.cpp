#include "BitmapPlusPlus.hpp"
#include <iostream>

int main(void)
{
	try
	{
		bmp::Bitmap image(512, 512);
		for (std::int32_t y = 0; y < image.Height(); ++y)
		{
			for (std::int32_t x = 0; x < image.Width(); ++x)
			{
				bmp::Pixel color =
				{
					static_cast<std::uint8_t>(std::rand() % 256),
					static_cast<std::uint8_t>(std::rand() % 256),
					static_cast<std::uint8_t>(std::rand() % 256)
				};
				image.Set(x, y, color);
			}
		}
		image.Save("image.bmp");
	}
	catch (const bmp::BitmapException& e)
	{
		std::cerr << "[BMP ERROR]: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}