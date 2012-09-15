//
//  bmp.hpp
//  ImageStreams
//
//  Created by Joshua Moerman on 9/7/12.
//  Copyright (c) 2012 Vadovas. All rights reserved.
//

#ifndef ImageStreams_bmp_hpp
#define ImageStreams_bmp_hpp

#include <ostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include "basics.hpp"

/*
 Note that a bmp file is vertically flipped.
 */

namespace bmp {
	// file header
	struct bitmap_file_header {
		uint32_t filesize;		// the size of the BMP file in bytes
		uint16_t creator1;		// reserved
		uint16_t creator2;		// reserved
		uint32_t bmp_offset;	// starting offset of image data
		
		template <typename DIBT, typename CT>
		bitmap_file_header(DIBT dib_header, CT color_table)
		: filesize(dib_header.bytes() + dib_header.header_sz + color_table.size() + 12 + 2)
		, creator1(0)
		, creator2(0)
		, bmp_offset(dib_header.header_sz + color_table.size() + 12 + 2) {}
		
		void write(std::ostream& out) const {
			out << "BM";
			out.write(reinterpret_cast<const char*>(this), 12);
		}
	};
	
	// the very basic bitmapcoreheader, no alpha supported
	struct bitmapcoreheader {
		uint32_t header_sz;
		uint16_t width;
		uint16_t height;
		uint16_t nplanes;
		uint16_t bitspp;
		
		bitmapcoreheader(uint16_t width, uint16_t height, uint16_t bitspp)
		: header_sz(sizeof(bitmapcoreheader))
		, width(width)
		, height(height)
		, nplanes(1)
		, bitspp(bitspp) {}
		
		void write(std::ostream& out) const {
			out.write(reinterpret_cast<const char*>(this), header_sz);
		}
		
		uint32_t bytes() const {
			return width*height*bitspp/8;
		}
	};
	
	// empty color table
	struct no_color_table {
		uint32_t size() const {
			return 0;
		};
		
		void write(std::ostream& out) const {}
	};
	
	// gray color table without alpha channel (eg for bitmapcoreheader)
	template <size_t N>
	struct gray_color_table {
		uint32_t size() const {
			// 2^N entries of 3 bytes
			return (2 << (N-1)) * 3;
		}
		
		void write(std::ostream& out) const {
			for(unsigned int i = 0; i < (2 << (N-1)); ++i){
				const char v = i;
				// more or less b g r.
				const char arr[3] = {v, v, v};
				out.write(arr, 3);
			}
		}
	};
	
	// when bpp > 8 there should be no color table, else there should be
	template <typename P>
	struct default_color_table : public std::conditional<pixel_formats::traits<P>::bits_per_pixel <= 8, gray_color_table<pixel_formats::traits<P>::bits_per_pixel>, no_color_table>::type
	{};
	
	template <typename P = pixel_formats::bgr, typename CT = default_color_table<P>>
	struct bitmap_stream {
		typedef P pixel;
		typedef CT color_table;
		
		bitmap_stream() = delete;
		bitmap_stream(bitmap_stream const &) = delete;
		bitmap_stream & operator=(bitmap_stream const &) = delete;
		
		// TODO: make this template on class level?
		// You can't use them in a ctor without template argument deduction...
		template <typename DIBT = bitmapcoreheader>
		bitmap_stream(uint16_t width, uint16_t height, std::string filename)
		: file(filename.c_str())
		, width(width)
		, height(height)
		, x(0)
		, y(0)
		{
			if(!file) throw std::runtime_error("bitmap file could not be opened.");
			
			DIBT dib_header(width, height, pixel_formats::traits<P>::bits_per_pixel);
			color_table ct;
			bitmap_file_header header(dib_header, ct);
			
			header.write(file);
			dib_header.write(file);
			ct.write(file);
		}
		
		bitmap_stream& operator<<(pixel const & p){
			if (y >= height) throw std::out_of_range("Writing BMP image out of bounds.");
			
			file.write((char const *)&p, sizeof(pixel));
			++x;
			if (x >= width){
				x = 0;
				++y;
			}
			return *this;
		}
		
		// TODO: destructor with checking / padding?
		
	private:
		std::ofstream file;
		
		uint16_t width;
		uint16_t height;
		
		uint16_t x;
		uint16_t y;
	};
}

#endif
