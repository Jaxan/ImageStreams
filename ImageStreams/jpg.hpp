//
//  jpg.hpp
//  ImageStreams
//
//  Created by Joshua Moerman on 9/15/12.
//  Copyright (c) 2012 Vadovas. All rights reserved.
//

#ifndef ImageStreams_jpg_hpp
#define ImageStreams_jpg_hpp

#include <stdio.h>
#include <stdexcept>
#include <vector>
#include <jpeglib.h>
#include "basics.hpp"

/*
 JPEG only supports 8bits or 24bits per pixel (eg grayscale or full color).
 Top to bottom
 */

namespace jpg{
	template <typename P>
	J_COLOR_SPACE color_space(){
		int num_colors = pixel_formats::traits<P>::num_colors;
		switch (num_colors) {
			case 1: return JCS_GRAYSCALE;
			case 3: return JCS_RGB;
			default: return JCS_UNKNOWN;
		}
	}
	
	template <typename P = pixel_formats::rgb>
	struct ostream{
		typedef P pixel;
		
		ostream() = delete;
		ostream(ostream const &) = delete;
		ostream & operator=(ostream const &) = delete;
		
		ostream(uint32_t width, uint32_t height, std::string filename)
		: fp(0)
		, cinfo()
		, jerr()
		, row(width)
		, x(0)
		{
			fp = fopen(filename.c_str(), "wb");
			if(!fp) throw std::runtime_error("Could not open file");
			
			cinfo.err = jpeg_std_error(&jerr);
			jpeg_create_compress(&cinfo);
			jpeg_stdio_dest(&cinfo, fp);
			
			cinfo.image_width      = width;
			cinfo.image_height     = height;
			cinfo.input_components = pixel_formats::traits<pixel>::num_colors;
			cinfo.in_color_space   = color_space<pixel>();
			
			jpeg_set_defaults(&cinfo);
			jpeg_set_quality(&cinfo, 95, true);	// quality in [0, 100], boolean indicates "force_baseline" (only matters when quality < 25)
			jpeg_start_compress(&cinfo, true);	// true means we will write completely
		}
		
		~ostream(){
			jpeg_finish_compress(&cinfo);
			fclose(fp);
			jpeg_destroy_compress(&cinfo);
		}
		
		ostream& operator<<(pixel const & p){
			row[x] = p;
			++x;
			if(x >= row.size()){
				// this will return the number of scanlines written
				unsigned char * ptr = reinterpret_cast<unsigned char *>(row.data());
				jpeg_write_scanlines(&cinfo, &ptr, 1);
				x = 0;
			}
			
			return *this;
		}
		
	private:
		FILE* fp;
		jpeg_compress_struct cinfo;
		jpeg_error_mgr jerr;
		
		std::vector<pixel> row;
		uint32_t x;
	};
	
	typedef ostream<> colored_ostream;
	typedef ostream<pixel_formats::gray> gray_ostream;
	
}

#endif
