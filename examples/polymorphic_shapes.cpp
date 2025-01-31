#include <iostream>
#include <filesystem>
#include "BitmapPlusPlus.hpp"

struct Shape {
  virtual ~Shape() = default;

  int x, y;
  bmp::Pixel color;

  Shape(int x, int y, bmp::Pixel color) : x(x), y(y), color(color) {
  }

  virtual void draw(bmp::Bitmap& image) = 0;
};

struct Rectangle : Shape {
  int width, height;

  Rectangle(int x, int y, int w, int h, bmp::Pixel color) : width(w), height(h), Shape(x, y, color) {
  }

  void draw(bmp::Bitmap& image) override {
    image.fill_rect(x, y, width, height, color);
  }
};

struct Triangle : Shape {
  int x2, y2, x3, y3;

  Triangle(int x1, int y1, int x2, int y2, int x3, int y3, bmp::Pixel color) : x2(x2), y2(y2), x3(x3), y3(y3), Shape(x1, y1, color) {
  }

  void draw(bmp::Bitmap& image) override {
    image.fill_triangle(x, y, x2, y2, x3, y3, color);
  }
};

struct Circle : Shape {
  int radius;

  Circle(int x, int y, int radius, bmp::Pixel color) : radius(radius), Shape(x, y, color) {
  }

  void draw(bmp::Bitmap& image) override {
    image.fill_circle(x, y, radius, color);
  }
};

int main() {
  try {
    bmp::Bitmap image(640, 256);
    bmp::Pixel background_color{bmp::Silver};
    image.clear(background_color);

    std::vector<Shape*> shapes{
      new Rectangle(20, 20, 180, 180, bmp::Pixel(0xa31d3a)),
      new Triangle(310, 20, 230, 200, 400, 200, bmp::Pixel(0x1a5096)),
      new Circle(500, 110, 90, bmp::Pixel(0x228035))};

    for (Shape* shape: shapes) {
      shape->draw(image);
      delete shape;
    }
    image.save(std::filesystem::path(BIN_DIR) / "polymorphic_shapes.bmp");

    return EXIT_SUCCESS;
  } catch (const bmp::Exception& e) {
    std::cerr << "[BMP ERROR]: " << e.what() << '\n';
    return EXIT_FAILURE;
  }
}
