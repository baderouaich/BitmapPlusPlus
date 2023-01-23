#include <iostream>
#include "BitmapPlusPlus.hpp"

int main()
{
    try
    {
        // 8x8 chess board
        bmp::Bitmap image(640, 640);
        const std::size_t board_dims = 8;
        const std::size_t rect_w = image.width() / board_dims;
        const std::size_t rect_h = image.height() / board_dims;

        // Iterate over rects
        bool is_white = true;
        for (std::size_t x = 0; x < image.width(); x += rect_w)
        {
            for (std::size_t y = 0; y < image.height(); y += rect_h)
            {
                bmp::Pixel color = is_white ? bmp::White : bmp::Black;
                // Fill rect
                for (size_t dx = x; dx < x + rect_w; dx++)
                {
                    for (size_t dy = y; dy < y + rect_h; dy++)
                    {
                        image.set(dx, dy, color);
                    }
                }
                is_white = !is_white;
            }
            is_white = !is_white;
        }

        // Save bitmap to file
        image.save("chess_board.bmp");

        return EXIT_SUCCESS;
    }
    catch (const bmp::Exception &e)
    {
        std::cerr << "[BMP ERROR]: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
}