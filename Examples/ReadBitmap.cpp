#include "BitmapPlusPlus.hpp"

int main(void)
{
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
	else
		return EXIT_FAILURE; 

	return EXIT_SUCCESS;
}