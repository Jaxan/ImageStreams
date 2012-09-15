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

template <typename ImageType>
void test4(std::string filename){
	size_t width = 1024;
	size_t height = 768;
	
	std::vector<int> r1(width, 0);
	std::vector<int> r2(width, 0);
	
	r1[width/2] = 1;
	
	ImageType image(width, height, filename);
	for(int y = 0; y < height; ++y){
		for(auto x : r1) image << typename ImageType::pixel((double)x);
		
		r1.swap(r2);
		for(int x = 1; x < width-1; ++x){
			r1[x] = (r2[x-1] || r2[x] || r2[x+1]) && !(r2[x-1] && r2[x] && r2[x+1]);
		}
	}
}

int main(int argc, const char * argv[]){
	test<png::png_stream<>>("test.png");
	test<bmp::bitmap_stream<>>("test.bmp");
	
	test2<png::png_stream<pixel_formats::gray>>("test_gray.png");
	test2<bmp::bitmap_stream<pixel_formats::gray>>("test_gray.bmp");
	
	test3<png::png_stream<pixel_formats::gray>>("test_3.png");
	test3<bmp::bitmap_stream<pixel_formats::gray>>("test_3.bmp");
	
	test4<png::png_stream<pixel_formats::gray>>("test_4.png");
	test4<bmp::bitmap_stream<pixel_formats::gray>>("test_4.bmp");
}

