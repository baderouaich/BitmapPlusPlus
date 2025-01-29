#include <filesystem>
#include "BitmapPlusPlus.hpp"

using namespace bmp;

int main() {
	// Create a 512x240 blank image
	Bitmap image(512, 240);
	image.clear(Pixel(0x25292e));

	/** Line **/
	// Draw a yellow line from position (250, 50) to position (500, 50)
	image.draw_line(250, 50, 500, 50, Yellow);

	/** Rectangle **/
	// Draw a red rectangle in position (10, 10) with size 100x100
	image.draw_rect(10, 10, 100, 100, Red);
	// Draw a white filled rectangle in position (120, 10) with size 100x100
	image.fill_rect(120, 10, 100, 100, White);

	/** Triangle **/
	image.draw_triangle(60, 120, 10, 220, 120, 220, Cyan);
	image.fill_triangle(180, 120, 130, 220, 245, 220, Magenta);

	/** Circle **/
	// Draw a non-filled Gray circle in position (300, 170) with 50 pixels radius
	image.draw_circle(300, 170, 50, Gray);
	// Draw a filled Lime circle in position (300, 170) with 50 pixels radius
	image.fill_circle(420, 170, 50, Lime);

	// Save bitmap
	image.save(std::filesystem::path(BIN_DIR) / "primitives.bmp");

	return EXIT_SUCCESS;
}
