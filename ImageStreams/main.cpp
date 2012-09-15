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
#include "jpg.hpp"
#include "basics.hpp"

#include <cmath>
#include <vector>
#include <array>
#include <algorithm>

template <typename ImageType>
void basic_colors(std::string filename) {
	ImageType image(8, 1, filename);
	std::array<double, 2> v = {0.0, 1.0};
	for(auto b : v) for(auto g : v) for(auto r : v) image << typename ImageType::pixel(r,g,b);
}

template <typename ImageType>
void xor_color(std::string filename){
	size_t size = 512;
	ImageType image(size, size, filename);
	
	for(int y = 0; y < size; ++y){
		for(int x = 0; x < size; ++x){
			image << typename ImageType::pixel(x ^ y, (2 * x) ^ y, x ^ (2 * y));
		}
	}
}

template <typename ImageType>
void xor_grad(std::string filename){
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
void noise(std::string filename){
	size_t size = 256;
	
	ImageType image(size, size, filename);
	
	for(int i = 0; i < size*size; ++i)
		image << typename ImageType::pixel(nice_rand());
}

template <typename ImageType>
void automata(std::string filename){
	size_t width = 1024;
	size_t height = 768;
	
	std::vector<int> r1(width, 0);
	std::vector<int> r2(width, 0);
	
	r1[width/3] = 1;
	
	ImageType image(width, height, filename);
	for(int y = 0; y < height; ++y){
		for(auto x : r1) image << typename ImageType::pixel((double)x);
		
		r1.swap(r2);
		for(int x = 1; x < width-1; ++x){
			r1[x] = (r2[x-1] || r2[x] || r2[x+1]) && !(r2[x-1] && r2[x] && r2[x+1]);
		}
	}
}

inline double logistic_step(double in, double c){
	return c * in * (1.0 - in);
}

template <typename ImageType>
void logistic(std::string filename) {
	size_t size = 800;
	double x = 0.5;
	double start = 3.7;
	double end = 4.0;
	
	ImageType image(size, size, filename);
	for(int i = 0; i < size*size; ++i){
		double c = start + i * (end - start) / double(size*size - 1);
		// I know; the order of evaluation is implementation defined
		image << typename ImageType::pixel(x = logistic_step(x, c), x = logistic_step(x, c), x = logistic_step(x, c));
	}
}

template <typename ImageType>
void logistic2(std::string filename) {
	size_t width = 1280;
	size_t height = 800;
	double start = 3.847;
	double end = 3.9;
	
	ImageType image(width, height, filename);
	for(int y = 0; y < height; ++y){
		double z = 0.5 * y / double(height - 1);
		for(int x = 0; x < width; ++x){
			double c = start + x * (end - start) / double(width - 1);
			image << typename ImageType::pixel(z = logistic_step(z, c), z = logistic_step(z, c), z = logistic_step(z, c));
		}
	}
}

inline int mandelbrot_thing(double const x, double const y){
	double zx = 0.0, zy = 0.0;
	for(int i = 0; i < 255; ++i){
		double t = zx;
		zx = zx*zx - zy*zy + x;
		zy = 2.0 * t * zy + y;
		
		if(zx*zx + zy*zy > 100) return 10*i;
	}
	return 0;
}

template <typename ImageType>
void mandelbrot(std::string filename) {
	size_t width = 1280;
	size_t height = 800;
	
	ImageType image(width, height, filename);
	for(int y = 0; y < height; ++y){
		double dy = y / double(height - 1) * 2.0 - 1.0;
		for(int x = 0; x < width; ++x){
			double dx = x / double(width - 1) * 2.0 * (width/double(height)) - 2.5;
			image << typename ImageType::pixel(mandelbrot_thing(dx, dy));
		}
	}
}

int main(int argc, const char * argv[]){
	#define test(fun, kind) \
	std::cout << "Testing " #fun << std::endl; \
	fun<png::kind ## _ostream>(#fun ".png"); \
	fun<bmp::kind ## _ostream>(#fun ".bmp"); \
	fun<jpg::kind ## _ostream>(#fun ".jpg")

	test(basic_colors, colored);
	test(xor_color, colored);
	test(logistic, colored);
	test(logistic2, colored);
	
	test(noise, gray);
	test(xor_grad, gray);
	test(automata, gray);
	test(mandelbrot, gray);
	
	#undef test
}

