#include "BitmapPlusPlus.hpp"

#define SAVE_BITMAP_TO_FILE 1
#define LOAD_BITMAP_FROM_FILE 0


int main(void)
{
#if SAVE_BITMAP_TO_FILE

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

#elif LOAD_BITMAP_FROM_FILE

	bmp::Bitmap image;
	if (image.Load("penguin.bmp"))
	{
		//Modify loaded image (makes half of the image black)
		for (std::int32_t y = 0; y < image.Height(); ++y)
		{
			for (std::int32_t x = 0; x < image.Width() / 2; ++x)
			{
				image.Set(x, y, bmp::Pixel::Black);
			}
		}
		//Save
		image.Save("modified-penguin.bmp");
	}
	
#endif

	return 0;
}