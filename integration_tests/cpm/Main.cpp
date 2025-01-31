#include <BitmapPlusPlus.hpp>

int main() {
  bmp::Bitmap bitmap(512, 512);
  for (auto& pixel : bitmap) {
    pixel = bmp::Pixel(42, 69, 96);
  }
  bitmap.save("demo.bmp");
}