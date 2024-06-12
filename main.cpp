#include "color.h"
#include "vec3.h"

#include <iostream>

const int IMAGE_WIDTH = 256;
const int IMAGE_HEIGHT = 256;

int main() {
	std::cout << "P3\n" << IMAGE_WIDTH << ' ' << IMAGE_HEIGHT << "\n255\n";

	for (int i = 0; i < IMAGE_HEIGHT; i++) {
		std::clog << "\rScanlines remaining: " << (IMAGE_HEIGHT - i) << ' ' << std::flush;
		for (int j = 0; j < IMAGE_WIDTH; j++) {
			auto pixel_color = color(double(j) / (IMAGE_WIDTH - 1), double(i) / (IMAGE_HEIGHT - 1), 0);
			write_color(std::cout, pixel_color);
		}
	}

	std::clog << "\rDone\n";

	return 0;
}