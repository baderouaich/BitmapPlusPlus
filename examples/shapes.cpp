#include <iostream>
#include "BitmapPlusPlus.hpp"

struct Shape
{
    int x, y;
    bmp::Pixel color;
    Shape(int x, int y, bmp::Pixel color) : x(x), y(y), color(color) {}

    void draw(bmp::Bitmap &image)
    {
        for (std::size_t dx = 0; dx < image.width(); dx++)
        {
            for (std::size_t dy = 0; dy < image.height(); dy++)
            {
                if (pixel_in_shape(dx, dy))
                    image.set(dx, dy, color);
            }
        }
    }
    virtual bool pixel_in_shape(int x, int y) { return false; };
};
struct Rectangle : Shape
{
    int width, height;
    Rectangle(int x, int y, int w, int h, bmp::Pixel color) : width(w), height(h), Shape(x, y, color) {}

    bool pixel_in_shape(int x, int y) override { return x >= this->x && x <= this->x + width && y >= this->y && y <= this->y + height; };
};
struct Triangle : Shape
{
    int x2, y2, x3, y3;
    Triangle(int x1, int y1, int x2, int y2, int x3, int y3, bmp::Pixel color) : x2(x2), y2(y2), x3(x3), y3(y3), Shape(x1, y1, color) {}

    bool pixel_in_shape(int x, int y) override
    {
        auto area = [](int x1, int y1, int x2, int y2, int x3, int y3)
        { return std::abs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0); };
        float A = area(this->x, this->y, x2, y2, x3, y3);
        float A1 = area(x, y, x2, y2, x3, y3);
        float A2 = area(this->x, this->y, x, y, x3, y3);
        float A3 = area(this->x, this->y, x2, y2, x, y);
        return (A == A1 + A2 + A3);
    }
};
struct Circle : Shape
{
    int radius;
    Circle(int x, int y, int radius, bmp::Pixel color) : radius(radius), Shape(x, y, color) {}

    bool pixel_in_shape(int x, int y) override
    {
        return ((x - this->x) * (x - this->x) + (y - this->y) * (y - this->y)) <= radius * radius;
    }
};

int main()
{
    try
    {
        bmp::Bitmap image(640, 256);
        bmp::Pixel background_color = {170, 170, 180};
        image.clear(background_color);

        std::vector<Shape *> shapes
        {
            new Rectangle(20, 20, 180, 180, bmp::Pixel(163, 24, 48)),
            new Triangle(310, 20, 230, 200, 400, 200, bmp::Pixel(21, 59, 148)),
            new Circle(500, 110, 90, bmp::Pixel(34, 128, 53))    
        };

        for (Shape *shape : shapes)
        {
            shape->draw(image);
            delete shape;
        }
        image.save("shapes.bmp");

        return EXIT_SUCCESS;
    }
    catch (const bmp::Exception &e)
    {
        std::cerr << "[BMP ERROR]: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
}