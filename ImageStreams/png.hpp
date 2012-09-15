//
//  png.hpp
//  ImageStreams
//
//  Created by Joshua Moerman on 9/7/12.
//  Copyright (c) 2012 Vadovas. All rights reserved.
//

#ifndef ImageStreams_png_hpp
#define ImageStreams_png_hpp

#include <stdio.h>
#include <stdexcept>
#include <vector>
#include <png.h>
#include "basics.hpp"

namespace png{
	template <typename P>
	int color_type(){
		int num_colors = pixel_formats::traits<P>::num_colors;
		switch (num_colors) {
			case 1: return PNG_COLOR_TYPE_GRAY;
			case 2: return PNG_COLOR_TYPE_GRAY_ALPHA;
			case 3: return PNG_COLOR_TYPE_RGB;
			case 4: return PNG_COLOR_TYPE_RGB_ALPHA;
			default: return PNG_COLOR_TYPE_RGB;
		}
	}
	
	template <typename P = pixel_formats::rgb>
	struct png_stream{
		typedef P pixel;
		
		png_stream() = delete;
		png_stream(png_stream const &) = delete;
		png_stream & operator=(png_stream const &) = delete;
		
		png_stream(uint32_t width, uint32_t height, std::string filename)
		: fp(0)
		, png_ptr(0)
		, info_ptr(0)
		, row(width)
		, x(0)
		{
			fp = fopen(filename.c_str(), "wb");
			if(!fp) throw std::runtime_error("Could not open file");
			
			png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
			if(!png_ptr) throw std::runtime_error("PNG structure could not be allocated");
			
			info_ptr = png_create_info_struct(png_ptr);
			if(!info_ptr) throw std::runtime_error("PNG information structure could not be allocated");
			
			png_init_io(png_ptr, fp);
			
			png_set_IHDR(png_ptr, info_ptr, width, height, pixel_formats::traits<pixel>::bits_per_color, color_type<pixel>(), PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
			
			png_set_compression_level(png_ptr, 9);
			
			png_write_info(png_ptr, info_ptr);
		}
		
		~png_stream(){
			// NOTE: the pnglib already checks for us whether all pixels are written
			png_write_end(png_ptr, info_ptr);
			png_destroy_info_struct(png_ptr, &info_ptr);
			fclose(fp);
		}
		
		png_stream& operator<<(pixel const & p){
			row[x] = p;
			++x;
			if(x >= row.size()){
				png_write_row(png_ptr, reinterpret_cast<unsigned char const*>(row.data()));
				x = 0;
			}
			
			return *this;
		}
		
	private:
		FILE* fp;
		png_structp png_ptr;
		png_infop info_ptr;
		
		std::vector<pixel> row;
		uint32_t x;
	};
}

#endif
