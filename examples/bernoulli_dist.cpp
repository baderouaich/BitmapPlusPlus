#include <iostream>
#include "BitmapPlusPlus.hpp"
#include <random>

int main() {
  try {
    // Generate a bitmap of 10% white and 90% black pixels
    bmp::Bitmap image(512, 512);

    std::random_device seed{};
    std::default_random_engine eng{seed()};
    std::bernoulli_distribution dist(0.10); // 10% White, 90% Black

    for (bmp::Pixel &pixel: image) {
      const bmp::Pixel color = dist(eng) ? bmp::White : bmp::Black;
      pixel = color;
    }

    image.save("bernoulli.bmp");

    return EXIT_SUCCESS;
  } catch (const bmp::Exception &e) {
    std::cerr << "[BMP ERROR]: " << e.what() << '\n';
    return EXIT_FAILURE;
  }
}
