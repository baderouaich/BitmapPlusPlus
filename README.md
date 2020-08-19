# Bitmap Plus Plus
Simple and Fast header only Bitmap (BMP) library

## Bitmap Type Supported
- 24 Bits Per Pixel

## Example
Create & Save Bitmap to file
<br>
```cpp
#include "BitmapPlusPlus.hpp"

int main(void)
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

	return 0;
}
```
![image](image.bmp)

<br>
Load  Bitmap from file, Modify and Save

![penguin](penguin.bmp)
<br>
```cpp
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

	return 0;
}
```
![modified-penguin](modified-penguin.bmp)


## Features and bugs
If you face any problems feel free to open an issue at the [issue tracker][tracker]. If you feel the extension is missing a feature, please raise a ticket on Github. Pull request are also welcome.

[tracker]: https://github.com/BaderEddineOuaich/BitmapPlusPlus/issues


