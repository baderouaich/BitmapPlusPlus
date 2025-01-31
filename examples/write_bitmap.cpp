#include "BitmapPlusPlus.hpp"
#include <random>
#include <filesystem>
#include <iostream>

static bmp::Pixel random_color() {
  static std::random_device seed{};
  static std::default_random_engine engine{seed()};
  std::uniform_int_distribution<std::int32_t> dist(0, 255);
  bmp::Pixel color{};
  color.r = dist(engine);
  color.g = dist(engine);
  color.b = dist(engine);
  return color;
}

int main() {
  try {
    // Create a 512x512 bitmap
    bmp::Bitmap image(512, 512);

    // Assign a random color to each pixel in the image
    for (bmp::Pixel &pixel: image) {
      pixel = random_color();
    }

    // Save bitmap to new file image.bmp
    image.save(std::filesystem::path(BIN_DIR) / "image.bmp");

    // And Voila!
    return EXIT_SUCCESS;
  } catch (const bmp::Exception &e) {
    std::cerr << "[BMP ERROR]: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
}
