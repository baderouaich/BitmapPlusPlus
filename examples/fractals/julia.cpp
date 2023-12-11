#include "BitmapPlusPlus.hpp"
#include "color_maps.inl"

int main(void) {
  bmp::Bitmap image(1280, 960);

  constexpr const std::uint16_t max_iterations = 300;

  constexpr const double cr = -0.70000;
  constexpr const double ci = 0.27015;

  double prevr, previ;

  for (std::int32_t y = 0; y < image.height(); ++y) {
    for (std::int32_t x = 0; x < image.width(); ++x) {
      double nextr = 1.5 * (2.0 * x / image.width() - 1.0);
      double nexti = (2.0 * y / image.height() - 1.0);

      for (std::uint16_t i = 0; i < max_iterations; ++i) {
        prevr = nextr;
        previ = nexti;

        nextr = prevr * prevr - previ * previ + cr;
        nexti = 2 * prevr * previ + ci;

        if (((nextr * nextr) + (nexti * nexti)) > 4) {
          const bmp::Pixel color = hsv_colormap[static_cast<std::size_t>((1000.0 * i) / max_iterations)];
          image.set(x, y, color);
          break;
        }
      }
    }
  }

  image.save("julia.bmp");

  return EXIT_SUCCESS;
}
