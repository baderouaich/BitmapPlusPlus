#include "BitmapPlusPlus.hpp"
#include <filesystem>
#include <iostream>
#include <variant>

struct Rectangle {
	int x, y;
	int width, height;
	bmp::Pixel color;

	Rectangle(int x, int y, int w, int h, bmp::Pixel color)
		: width(w), height(h), x(x), y(y), color(color) {
	}

	void draw(bmp::Bitmap& image) const {
		image.fill_rect(x, y, width, height, color);
	}
};

struct Triangle {
	int x1, y1, x2, y2, x3, y3;
	bmp::Pixel color;


	Triangle(int x1, int y1, int x2, int y2, int x3, int y3, bmp::Pixel color)
		: x1(x1), y1(y1), x2(x2), y2(y2), x3(x3),
		y3(y3),
		color(color) {
	}

	void draw(bmp::Bitmap& image) const {
		image.fill_triangle(x1, y1, x2, y2, x3, y3, color);
	}
};

struct Circle {
	int x, y;
	int radius;
	bmp::Pixel color;

	Circle(int x, int y, int radius, bmp::Pixel color)
		: radius(radius), x(x), y(y), color(color) {
	}

	void draw(bmp::Bitmap& image) const {
		image.fill_circle(x, y, radius, color);
	}
};

struct ShapeDrawer {
	bmp::Bitmap& bitmap;

	explicit ShapeDrawer(bmp::Bitmap& bitmap)
		: bitmap(bitmap) {
	}

	void operator()(const Rectangle& rectangle) const {
		const auto& [x, y, width, height, color] = rectangle;
		bitmap.fill_rect(x, y, width, height, color);
	}

	void operator()(const Triangle& triangle) const {
		const auto& [x1, y1, x2, y2, x3, y3, color] = triangle;
		bitmap.fill_triangle(x1, y1, x2, y2, x3, y3, color);
	}

	void operator()(const Circle& circle) const {
		const auto& [x, y, radius, color] = circle;
		bitmap.fill_circle(x, y, radius, color);
	}
};


int main() {
	try {
		bmp::Bitmap image(640, 256);
		image.clear(bmp::Teal);

		ShapeDrawer drawer{ image };

		// No OOP polymorphism + faster
		// for more details, watch this great talk by Klaus Iglberger: https://www.youtube.com/watch?v=m3UmABVf55g
		using Shape = std::variant<Rectangle, Triangle, Circle>;
		Shape shapes[] = {
		  Rectangle(20, 20, 180, 180, bmp::Gold),
		  Triangle(310, 20, 230, 200, 400, 200, bmp::Violet),
		  Circle(500, 110, 90, bmp::Coral)
		};

		for (Shape& shape : shapes) {
			std::visit(drawer, shape); // visit will call the appropriate ShapeDrawer::operator() for each shape
		}
		image.save(std::filesystem::path(BIN_DIR) / "variant_shapes.bmp");

		return EXIT_SUCCESS;
	}
	catch (const bmp::Exception& e) {
		std::cerr << "[BMP ERROR]: " << e.what() << '\n';
		return EXIT_FAILURE;
	}
}
