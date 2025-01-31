#include "BitmapPlusPlus.hpp"
#include <iostream>
#include <filesystem>

int main() {
  try {
    bmp::Bitmap image;

    // Load penguin.bmp bitmap
    image.load(std::filesystem::path(ROOT_DIR) / "images" / "penguin.bmp");

    // Modify loaded image (makes half of the image black)
    for (std::int32_t y = 0; y < image.height(); ++y) {
      for (std::int32_t x = 0; x < image.width() / 2; ++x) {
        image.set(x, y, bmp::Black);
      }
    }

    // Save
    image.save(std::filesystem::path(BIN_DIR) / "modified-penguin.bmp");

    return EXIT_SUCCESS;
  } catch (const bmp::Exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
}
