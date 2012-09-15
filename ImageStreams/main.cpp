//
//  main.cpp
//  ImageStreams
//
//  Created by Joshua Moerman on 9/7/12.
//  Copyright (c) 2012 Vadovas. All rights reserved.
//

#include <iostream>

#include "png.hpp"
#include "bmp.hpp"
#include "basics.hpp"

#include <cmath>
#include <vector>
#include <algorithm>

template <typename ImageType>
void test(std::string filename){
	size_t size = 512;
	ImageType image(size, size, filename);
	
	for(int y = 0; y < size; ++y){
		for(int x = 0; x < size; ++x){
			image << typename ImageType::pixel(x ^ y, (2 * x) ^ y, x ^ (2 * y));
		}
	}
}

template <typename ImageType>
void test2(std::string filename){
	size_t size = 256;
	ImageType image(size, size, filename);
	
	for(int y = 0; y < size; ++y){
		for(int x = 0; x < size; ++x){
			if(x < size/2)
				image << typename ImageType::pixel(x ^ y);
			else
				image << typename ImageType::pixel(y);
		}
	}
}

inline double nice_rand(){
	return rand() / double(RAND_MAX);
}

template <typename ImageType>
void test3(std::string filename){
	size_t size = 256;
	
	ImageType image(size, size, filename);
	
	for(int i = 0; i < size*size; ++i)
		image << typename ImageType::pixel(nice_rand());
}

int main(int argc, const char * argv[]){
	test<png::png_stream<>>("test.png");
	test<bmp::bitmap_stream<>>("test.bmp");
	
	test2<png::png_stream<pixel_formats::gray>>("test_gray.png");
	test2<bmp::bitmap_stream<pixel_formats::gray>>("test_gray.bmp");
	
	test3<png::png_stream<pixel_formats::gray>>("test_3.png");
	test3<bmp::bitmap_stream<pixel_formats::gray>>("test_3.bmp");
}

