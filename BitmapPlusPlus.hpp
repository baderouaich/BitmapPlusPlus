#ifndef BITMAP_PLUSPLUS_H
#define BITMAP_PLUSPLUS_H

#include <cstdint>
#include <fstream>
#include <vector>


namespace bmp
{
#pragma pack(push, 1)
	struct BitmapHeader
	{
		/* Bitmap file header structure */
		std::uint16_t buffer_type;			/* Magic number for file always BM which is 0x4D42 */
		std::uint32_t buffer_size;			/* Size of file */
		std::uint16_t buffer_reserved1;		/* Reserved */
		std::uint16_t buffer_reserved2;		/* Reserved */
		std::uint32_t buffer_off_bits;		/* Offset to bitmap data */
		/* Bitmap file info structure */
		std::uint32_t size;			  /* Size of info header */
		std::int32_t  width;			  /* Width of image */
		std::int32_t  height;		  /* Height of image */
		std::uint16_t planes;		  /* Number of color planes */
		std::uint16_t bits_per_pixel;  /* Number of bits per pixel */
		std::uint32_t compression;	  /* Type of compression to use */
		std::uint32_t size_image;	  /* Size of image data */
		std::int32_t  xpels_per_meter; /* X pixels per meter */
		std::int32_t  ypels_per_meter; /* Y pixels per meter */
		std::uint32_t clr_used;		  /* Number of colors used */
		std::uint32_t clr_important;	  /* Number of important colors */
	};
	static_assert(sizeof(BitmapHeader) == 54, "Bitmap header size must be 54 bytes");

	struct Pixel
	{
		std::uint8_t r{ 0 };			/* Blue value */
		std::uint8_t g{ 0 };			/* Green value */
		std::uint8_t b{ 0 };			/* Red value */

		constexpr Pixel() = default;
		explicit constexpr Pixel(std::uint8_t rgb) noexcept : r(rgb), g(rgb), b(rgb) {}
		constexpr Pixel(std::uint8_t _r, std::uint8_t _g, std::uint8_t _b) noexcept : r(_r), g(_g), b(_b) {}

		static const Pixel Black, White, Red, Green, Blue;
	};
	static_assert(sizeof(Pixel) == 3, "Bitmap Pixel size must be 3 bytes");
#pragma pack(pop)


	const Pixel Pixel::Black = { std::uint8_t(0), std::uint8_t(0), std::uint8_t(0) };
	const Pixel Pixel::White = { std::uint8_t(255), std::uint8_t(255), std::uint8_t(255) };
	const Pixel Pixel::Red   = { std::uint8_t(255), std::uint8_t(0), std::uint8_t(0) };
	const Pixel Pixel::Green = { std::uint8_t(0), std::uint8_t(255), std::uint8_t(0) };
	const Pixel Pixel::Blue  = { std::uint8_t(0), std::uint8_t(0), std::uint8_t(255) };

	//Supporting only 24 bits per pixel bmp type
	static constexpr const auto BITMAP_BUFFER_TYPE = 0x4D42;

	class Bitmap
	{
	public:
		Bitmap() noexcept
			:
			m_pixels(), 
			m_width(0),
			m_height(0)
		{}
		Bitmap(std::int32_t width, std::int32_t height) noexcept
			:
			m_pixels(width * height),
			m_width(width),
			m_height(height)
		{}
		~Bitmap()
		{}

	public: //Accessors
		/*
		*	Returns the width of the Bitmap image
		*/
		constexpr std::int32_t Width() const noexcept { return m_width; }

		/*
		*	Returns the height of the Bitmap image
		*/
		constexpr std::int32_t Height() const noexcept { return m_height; }

		/*
		*	Clears Bitmap pixels with an rgb color
		*/
		void Clear(const Pixel& v = Pixel::Black)
		{
			std::fill(m_pixels.begin(), m_pixels.end(), v);
		}

		/*
		*	Operators
		*/
		const Pixel& operator[](const std::size_t i) const { return m_pixels[i]; }
		Pixel& operator[](const std::size_t i) { return m_pixels[i]; }
		bool operator!() const noexcept { return (m_pixels.size() == 0) || (m_width == 0) || (m_height == 0); }
		constexpr bool operator==(const Bitmap& image) const
		{
			if(this != &image)
				return
					(m_width == image.m_width) &&
					(m_height == image.m_height) &&
					(memcmp(m_pixels.data(), image.m_pixels.data(), sizeof(Pixel) * m_pixels.size()) == 0);
			return true;
		}
		constexpr bool operator!=(const Bitmap& image) const { return !(*this == image); }
		Bitmap& operator=(const Bitmap& image)
		{
			if (this != &image)
			{
				m_width = image.m_width;
				m_height = image.m_height;
				m_pixels = std::move(image.m_pixels);
			}
			return *this;
		}

	public: //Modificators
		/*
		*	Sets rgb color to pixel at position x,y
		*	prints error message to stderr if x,y coords are out of bounds.
		*/
		void Set(std::int32_t x, std::int32_t y, const Pixel& color) noexcept
		{
			if (!inBounds(x, y))
			{
				fprintf(stderr, "[BMP ERROR]: Bitmap::Set(%d, %d) out of bounds.\n", x, y);
				return;
			}
			m_pixels[IX(x, y)] = color;
		}

		/*
		*	Saves Bitmap pixels into a file
		*/
		bool Save(const std::string& filename)
		{
			const std::int32_t  row_size = m_width * 3 + m_width % 4;
			const std::uint32_t bitmap_size = row_size * m_height;
			const BitmapHeader header =
			{
				BITMAP_BUFFER_TYPE,
				static_cast<std::uint32_t>(bitmap_size + sizeof(BitmapHeader)),
				0,
				0,
				sizeof(BitmapHeader),
				40,
				m_width,
				m_height,
				1,
				24,
				0,
				bitmap_size,
				0,
				0,
				0,
				0
			};

			if (std::ofstream ofs{ filename, std::ios_base::binary })
			{
				//Write Header
				ofs.write(reinterpret_cast<const char*>(&header), sizeof(header));

				//Write Pixels
				std::vector<std::uint8_t> line(row_size);
				for (std::int32_t y = m_height - 1; y >= 0; --y)
				{
					std::size_t i = 0;
					for (std::int32_t x = 0; x < m_width; ++x)
					{
						const Pixel& color = m_pixels[IX(x, y)];
						line[i++] = color.b;
						line[i++] = color.g;
						line[i++] = color.r;
					}
					ofs.write(reinterpret_cast<const char*>(line.data()), line.size());
				}

				
				/*
				* @Causes problem: rgb values should be reversed to bgr
				* ofs.write(reinterpret_cast<const char*>(m_pixels.data()), sizeof(Color) * m_pixels.size());
				*/

				//Close File
				ofs.close();

				return true;
			}
			else
			{
				fprintf(stderr, "[Bitmap ERROR]: Bitmap::Save(\"%s\") Failed to save data to file.\n", filename.c_str());
				return false;
			}
		}


		/*
		*	Loads Bitmap from file
		*/
		bool Load(const std::string& filename)
		{
			m_pixels.clear();

			if (std::ifstream ifs{ filename, std::ios_base::binary })
			{
				//Read Header
				std::unique_ptr<BitmapHeader> header = std::make_unique<BitmapHeader>();
				ifs.read(reinterpret_cast<char*>(header.get()), sizeof(BitmapHeader));

				//Check if the Bitmap file is valid
				if (header->buffer_type != BITMAP_BUFFER_TYPE)
				{
					ifs.close();
					fprintf(stderr, "[Bitmap ERROR]: Bitmap::Load(\"%s\") Unrecognized file format.\n", filename.c_str());
					return false;

				}
				//Check if the Bitmap file has 24 bits per pixel
				if (header->bits_per_pixel != 24)
				{
					ifs.close();
					fprintf(stderr, "[Bitmap ERROR]: Bitmap::Load(\"%s\") Only 24 bits per pixel supported.\n", filename.c_str());
					return false;
				}


				//Set width & height 
				m_width = header->width;
				m_height = header->height;

				//Resize pixels size
				m_pixels.resize(m_width * m_height, Pixel::Black);

				//Read Bitmap pixels
				const std::int32_t  row_size = m_width * 3 + m_width % 4;
				std::vector<std::uint8_t> line(row_size);
				for (std::int32_t y = m_height - 1; y >= 0; --y)
				{
					ifs.read(reinterpret_cast<char*>(line.data()), line.size());
					std::size_t i = 0;
					for (std::int32_t x = 0; x < m_width; ++x)
					{
						Pixel color;
						color.b = line[i++];
						color.g = line[i++];
						color.r = line[i++];
						this->Set(x, y, color);
					}
				}

				//Close file
				ifs.close();

				return true;
			}
			else
			{
				fprintf(stderr, "[Bitmap ERROR]: Bitmap::Load(\"%s\") Failed to load Bitmap from file.\n", filename.c_str());
				return false;
			}
		}

	private:
		std::vector<Pixel> m_pixels;
		std::int32_t m_width{ 0 };
		std::int32_t m_height{ 0 };


	private: //Utils
		/*
		*	Converts 2D x,y coords into 1D index
		*/
		constexpr const std::size_t IX(const std::int32_t& x, const std::int32_t& y) const noexcept
		{
			return static_cast<std::size_t>(x + m_width * y);
		}

		/*
		*	Returns true if x,y coords are within boundaries
		*/
		constexpr bool inBounds(const std::int32_t& x, const std::int32_t& y) const noexcept
		{
			return (x >= 0) && (x < m_width) && (y >= 0) && (y < m_height);
		}

		/*
		*	Converts double rgb value into uint8
		*/
		constexpr std::uint8_t toUint8(const double& x) noexcept
		{
			return x >= 1.0 ? 255 : x <= 0.0 ? 0 : static_cast<std::uint8_t>(x * 255.0 + 0.5);
		}

	};
}
#endif // !BITMAP_PLUSPLUS_H