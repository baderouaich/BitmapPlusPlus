#include "BitmapPlusPlus.hpp"

int main(void) {
  try {
    bmp::Bitmap image;

    // Load penguin.bmp bitmap
    image.load("penguin.bmp");

    // Modify loaded image (makes half of the image black)
    for (std::int32_t y = 0; y < image.height(); ++y) {
      for (std::int32_t x = 0; x < image.width() / 2; ++x) {
        image.set(x, y, bmp::Black);
      }
    }

    // Save
    image.save("modified-penguin.bmp");

    return EXIT_SUCCESS;
  }
  catch (const bmp::Exception &e) {
    return EXIT_FAILURE;
  }
}