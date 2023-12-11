#include "BitmapPlusPlus.hpp"
#include "color_maps.inl"
#include <cmath>

int main(void) {
  bmp::Bitmap image(1280, 960);

  double cr, ci;
  double nextr, nexti;
  double prevr, previ;
  constexpr const std::uint16_t max_iterations = 3000;

  for (std::int32_t y = 0; y < image.height(); ++y) {
    for (std::int32_t x = 0; x < image.width(); ++x) {
      cr = 1.5 * (2.0 * x / image.width() - 1.0) - 0.5;
      ci = (2.0 * y / image.height() - 1.0);

      nextr = nexti = 0;
      prevr = previ = 0;

      for (std::uint16_t i = 0; i < max_iterations; ++i) {
        prevr = nextr;
        previ = nexti;

        nextr = prevr * prevr - previ * previ + cr;
        nexti = 2 * prevr * previ + ci;

        if (((nextr * nextr) + (nexti * nexti)) > 4) {
          const double z = sqrt(nextr * nextr + nexti * nexti);

          // https://en.wikipedia.org/wiki/Mandelbrot_set#Continuous_.28smooth.29_coloring
          const std::uint32_t index = static_cast<std::uint32_t>(1000.0 * log2(1.75 + i - log2(log2(z))) /
                                                                 log2(max_iterations));

          image.set(x, y, jet_colormap[index]);

          break;
        }
      }
    }
  }

  image.save("mandelbrot.bmp");

  return EXIT_SUCCESS;
}