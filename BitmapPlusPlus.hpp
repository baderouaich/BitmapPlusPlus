#pragma once
#ifndef BITMAP_PLUSPLUS_H
#define BITMAP_PLUSPLUS_H

#include <fstream>      // std::ofstream std::ifstream
#include <vector>       // std::vector
#include <memory>       // std::unique_ptr
#include <algorithm>    // std::fill
#include <cstdint>      // std::int*_t
#include <cstddef>		// std::size_t
#include <string>       // std::string
#include <cstring>      // std::memcmp
#include <exception>    // std::exception

namespace bmp
{
	// Magic number for Bitmap .bmp 24 bpp files (24/8 = 3 = rgb colors only)
	static constexpr const std::uint16_t BITMAP_BUFFER_MAGIC = 0x4D42;

#pragma pack(push, 1)
	struct BitmapHeader
	{
		/* Bitmap file header structure */
		std::uint16_t magic;           /* Magic number for file always BM which is 0x4D42 */
		std::uint32_t file_size;       /* Size of file */
		std::uint16_t reserved1;       /* Reserved */
		std::uint16_t reserved2;       /* Reserved */
		std::uint32_t offset_bits;     /* Offset to bitmap data */
		/* Bitmap file info structure */
		std::uint32_t size;            /* Size of info header */
		std::int32_t  width;           /* Width of image */
		std::int32_t  height;          /* Height of image */
		std::uint16_t planes;          /* Number of color planes */
		std::uint16_t bits_per_pixel;  /* Number of bits per pixel */
		std::uint32_t compression;     /* Type of compression to use */
		std::uint32_t size_image;      /* Size of image data */
		std::int32_t  xpels_per_meter; /* X pixels per meter */
		std::int32_t  ypels_per_meter; /* Y pixels per meter */
		std::uint32_t clr_used;        /* Number of colors used */
		std::uint32_t clr_important;   /* Number of important colors */
	};
	static_assert(sizeof(BitmapHeader) == 54, "Bitmap header size must be 54 bytes");

	struct Pixel
	{
		std::uint8_t r{ 0 };			/* Blue value */
		std::uint8_t g{ 0 };			/* Green value */
		std::uint8_t b{ 0 };			/* Red value */

		constexpr Pixel() noexcept = default;
		explicit constexpr Pixel(std::uint8_t rgb) noexcept : r(rgb), g(rgb), b(rgb) {}
		constexpr Pixel(std::uint8_t red, std::uint8_t green, std::uint8_t blue) noexcept : r(red), g(green), b(blue) {}
	};
	static_assert(sizeof(Pixel) == 3, "Bitmap Pixel size must be 3 bytes");
#pragma pack(pop)

	static constexpr const Pixel Black{ std::uint8_t(0),	std::uint8_t(0),	std::uint8_t(0) };
	static constexpr const Pixel White{ std::uint8_t(255),	std::uint8_t(255),	std::uint8_t(255) };
	static constexpr const Pixel Red{ std::uint8_t(255),	std::uint8_t(0),	std::uint8_t(0) };
	static constexpr const Pixel Green{ std::uint8_t(0),	std::uint8_t(255),	std::uint8_t(0) };
	static constexpr const Pixel Blue{ std::uint8_t(0),	std::uint8_t(0),	std::uint8_t(255) };


	class Exception : public std::exception
	{
	public:
		Exception(const std::string& message) : m_msg(message) { }
		virtual const char* what() const noexcept { return m_msg.c_str(); }
	private:
		const std::string m_msg;
	};

	class Bitmap
	{
	public:
		Bitmap() noexcept
			:
			m_pixels(),
			m_width(0),
			m_height(0)
		{}
		Bitmap(const std::string& filename)
			:
			m_pixels(),
			m_width(0),
			m_height(0)
		{
			Load(filename);
		}
		Bitmap(const std::int32_t width, const std::int32_t height)
			:
			m_pixels(static_cast<std::size_t>(width) * static_cast<std::size_t>(height)),
			m_width(width),
			m_height(height)
		{
			if (width == 0 || height == 0)
				throw Exception("Bitmap width and height must be > 0");
		}
		Bitmap(const Bitmap& other) // Copy Constructor
			:
			m_width(other.m_width),
			m_height(other.m_height),
			m_pixels(other.m_pixels)
		{}
		virtual ~Bitmap() noexcept
		{
			m_pixels.clear();
		}

	public:  /* Accessors */
		/**
		*	Get pixel at position x,y
		*/
		Pixel& Get(const std::int32_t x, const std::int32_t y)
		{
			if (!inBounds(x, y))
				throw Exception("Bitmap::Get(" + std::to_string(x) + ", " + std::to_string(y) + "): x,y out of bounds");
			return m_pixels[IX(x, y)];
		}

		/**
		*	Get const pixel at position x,y
		*/
		const Pixel& Get(const std::int32_t x, const std::int32_t y) const
		{
			if (!inBounds(x, y))
				throw Exception("Bitmap::Get(" + std::to_string(x) + ", " + std::to_string(y) + "): x,y out of bounds");
			return m_pixels[IX(x, y)];
		}

		/**
		*	Returns the width of the Bitmap image
		*/
		std::int32_t Width() const noexcept { return m_width; }

		/**
		*	Returns the height of the Bitmap image
		*/
		std::int32_t Height() const noexcept { return m_height; }

		/**
		*	Clears Bitmap pixels with an rgb color
		*/
		void Clear(const Pixel pixel = Black)
		{
			std::fill(m_pixels.begin(), m_pixels.end(), pixel);
		}

	public: /* Operators */
		const Pixel& operator[](const std::size_t i) const { return m_pixels[i]; }
		Pixel& operator[](const std::size_t i) { return m_pixels[i]; }
		bool operator!() const noexcept { return (m_pixels.size() == 0) || (m_width == 0) || (m_height == 0); }
		operator bool() const noexcept { return !(*this); }
		bool operator==(const Bitmap& image) const
		{
			if (this != &image)
			{
				return
					(m_width == image.m_width) &&
					(m_height == image.m_height) &&
					(std::memcmp(m_pixels.data(), image.m_pixels.data(), sizeof(Pixel) * m_pixels.size()) == 0);
			}
			return true;
		}
		bool operator!=(const Bitmap& image) const { return !(*this == image); }
		Bitmap& operator=(const Bitmap& image) // Move assignment operator
		{
			if (this != &image)
			{
				m_width = image.m_width;
				m_height = image.m_height;
				m_pixels = image.m_pixels;
			}
			return *this;
		}

	public: /** foreach iterators access */
		std::vector<Pixel>::iterator begin() noexcept { return m_pixels.begin(); }
		std::vector<Pixel>::iterator end() noexcept { return m_pixels.end(); }
		std::vector<Pixel>::const_iterator cbegin() const noexcept { return m_pixels.cbegin(); }
		std::vector<Pixel>::const_iterator cend() const noexcept { return m_pixels.cend(); }
		std::vector<Pixel>::reverse_iterator rbegin() noexcept { return m_pixels.rbegin(); }
		std::vector<Pixel>::reverse_iterator rend() noexcept { return m_pixels.rend(); }
		std::vector<Pixel>::const_reverse_iterator crbegin() const noexcept { return m_pixels.crbegin(); }
		std::vector<Pixel>::const_reverse_iterator crend() const noexcept { return m_pixels.crend(); }

	public: /* Modifiers */
		/**
		*	Sets rgb color to pixel at position x,y
		*   @throws bmp::Exception on error
		*/
		void Set(const std::int32_t x, const std::int32_t y, const Pixel color)
		{
			if (!inBounds(x, y))
			{
				throw Exception("Bitmap::Set(" + std::to_string(x) + ", " + std::to_string(y) + "): x,y out of bounds");
			}
			m_pixels[IX(x, y)] = color;
		}

		/**
		*	Saves Bitmap pixels into a file
		*   @throws bmp::Exception on error
		*/
		void Save(const std::string& filename)
		{
			// Calculate row and bitmap size
			const std::int32_t  row_size = m_width * 3 + m_width % 4;
			const std::uint32_t bitmap_size = row_size * m_height;

			// Construct bitmap header
			BitmapHeader header{};
			/* Bitmap file header structure */
			header.magic = BITMAP_BUFFER_MAGIC;
			header.file_size = bitmap_size + sizeof(BitmapHeader);
			header.reserved1 = 0;
			header.reserved2 = 0;
			header.offset_bits = sizeof(BitmapHeader);
			/* Bitmap file info structure */
			header.size = 40;
			header.width = m_width;
			header.height = m_height;
			header.planes = 1;
			header.bits_per_pixel = sizeof(Pixel) * 8; // 24bpp
			header.compression = 0;
			header.size_image = bitmap_size;
			header.xpels_per_meter = 0;
			header.ypels_per_meter = 0;
			header.clr_used = 0;
			header.clr_important = 0;

			// Save bitmap to output file
			if (std::ofstream ofs{ filename, std::ios::binary })
			{
				// Write Header
				ofs.write(reinterpret_cast<const char*>(&header), sizeof(BitmapHeader));

				// Write Pixels
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

				// Close File
				ofs.close();
			}
			else
				throw Exception("Bitmap::Save(\"" + filename + "\"): Failed to save pixels to file.");
		}

		/**
		*	Loads Bitmap from file
		*   @throws bmp::Exception on error
		*/
		void Load(const std::string& filename)
		{
			m_pixels.clear();

			if (std::ifstream ifs{ filename, std::ios::binary })
			{
				// Read Header
				std::unique_ptr<BitmapHeader> header(new BitmapHeader());
				ifs.read(reinterpret_cast<char*>(header.get()), sizeof(BitmapHeader));

				// Check if Bitmap file is valid
				if (header->magic != BITMAP_BUFFER_MAGIC)
				{
					ifs.close();
					throw Exception("Bitmap::Load(\"" + filename + "\"): Unrecognized file format.");

				}
				// Check if the Bitmap file has 24 bits per pixel (for now supporting only 24bpp bitmaps)
				if (header->bits_per_pixel != 24)
				{
					ifs.close();
					throw Exception("Bitmap::Load(\"" + filename + "\"): Only 24 bits per pixel bitmaps supported.");
				}

				// Set width & height 
				m_width = header->width;
				m_height = header->height;

				// Resize pixels size
				m_pixels.resize(static_cast<std::size_t>(m_width) * static_cast<std::size_t>(m_height), Black);

				// Read Bitmap pixels
				const std::int32_t row_size = m_width * 3 + m_width % 4;
				std::vector<std::uint8_t> line(row_size);
				for (std::int32_t y = m_height - 1; y >= 0; --y)
				{
					ifs.read(reinterpret_cast<char*>(line.data()), line.size());
					std::size_t i = 0;
					for (std::int32_t x = 0; x < m_width; ++x)
					{
						Pixel color{};
						color.b = line[i++];
						color.g = line[i++];
						color.r = line[i++];
						m_pixels[IX(x, y)] = color; //this->Set(x, y, color);
					}
				}

				// Close file
				ifs.close();
			}
			else
				throw Exception("Bitmap::Load(\"" + filename + "\"): Failed to load bitmap pixels from file.");
		}

	private: /* Utils */
		/**
		*	Converts 2D x,y coords into 1D index
		*/
		constexpr std::size_t IX(const std::int32_t x, const std::int32_t y) const noexcept
		{
			return static_cast<std::size_t>(x) + static_cast<std::size_t>(m_width) * static_cast<std::size_t>(y);
		}

		/**
		*	Returns true if x,y coords are within boundaries
		*/
		constexpr bool inBounds(const std::int32_t x, const std::int32_t y) const noexcept
		{
			return (x >= 0) && (x < m_width) && (y >= 0) && (y < m_height);
		}

		/**
		*	Converts double rgb value into uint8
		*/
		constexpr std::uint8_t toUint8(const double x) noexcept
		{
			return x >= 1.0 ? 255 : x <= 0.0 ? 0 : static_cast<std::uint8_t>(x * 255.0 + 0.5);
		}

	private:
		std::vector<Pixel> m_pixels;
		std::int32_t m_width;
		std::int32_t m_height;
	};
}
#endif // !BITMAP_PLUSPLUS_H
