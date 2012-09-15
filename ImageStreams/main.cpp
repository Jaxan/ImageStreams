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
		double c = start + i * (end - start) / (size*size);
		// I know; the order of evaluation is implementation defined
		image << typename ImageType::pixel(x = logistic_step(x, c), x = logistic_step(x, c), x = logistic_step(x, c));
	}
}

int main(int argc, const char * argv[]){
	xor_color<png::colored_ostream>("xor_color.png");
	xor_color<bmp::colored_ostream>("xor_color.bmp");
	
	xor_grad<png::gray_ostream>("xor_grad.png");
	xor_grad<bmp::gray_ostream>("xor_grad.bmp");
	
	noise<png::gray_ostream>("noise.png");
	noise<bmp::gray_ostream>("noise.bmp");
	
	automata<png::gray_ostream>("automata.png");
	automata<bmp::gray_ostream>("automata.bmp");
	
	logistic<png::colored_ostream>("logistic.png");
	logistic<bmp::colored_ostream>("logistic.bmp");
}

