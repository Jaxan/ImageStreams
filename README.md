ImageStreams
============

Motivation
----------
Some time ago I had a project where I rendered something to a high quality buffer, which I wanted to export as a png. I used libpng++ for that, the downside was that this library allocated the image again (in a different format). But the only thing I wanted to do is convert my format to another format (pixel by pixel), so this allocation seemed redundant. That's why I wanted a image stream library.

Features
--------
* Output streams for bmp, png and jpg image files
* Grayscale and color
* Low memory usage (because it streams)
* Easy to use

Usage
-----
```C++
png::gray_ostream image(1024, 768, "test.png");
for(int y = 0; y < 768; ++y){
	for(int x = 0; x < 1024; ++x){
		image << png::gray_ostream::pixel(some_generator(x,y));
	}
}
// done, image is automatically saved (with RAII)
```

There are more examples in the main.cpp file.

Dependencies
------------
For the png format libpng is required. For the jpg format libjpeg or libjpeg-turbo is required. The bmp format does not depend on anything, so you can output images without other libraries :D.

TODO
----
* Make output iterators
* Input streams (plus iterators)
* Add alpha support (where possible)
* Some interface for parameters (eg. jpg quality)
* Add appropriate license
* Really make it a library (ie. restructure directories)
