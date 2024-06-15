#include "color.h"
#include "vec3.h"
#include "ray.h"

#include <iostream>

double hit_circle(const point3& center, double radius, const ray& r) {
	vec3 origin_center = center - r.origin();
	//quadratic formula
	auto a = r.direction().length_squeared();
	auto h = dot(r.direction(), origin_center);
	auto c = origin_center.length_squeared() - (radius * radius);
	auto discriminant = (h * h) - (a * c);
	
	if (discriminant < 0.0) {
		return -1.0;
	}
	else {
		return (h - sqrt(discriminant)) / a;
	}

}

color sky_color(const ray& r) {
	vec3 unit_direction = unit_vector(r.direction());
	//linear blend
	auto a = 0.5 * (unit_direction.y() + 1.0);
	return ((1.0 - a) * color(1.0, 1.0, 1.0)) + (a * color(0.5, 0.7, 1.0));
}

color ray_color(const ray& r) {
	auto t = hit_circle(point3(0, 0, -1), 0.5, r);
	if (t > 0.0) {
		vec3 surface_normal = unit_vector(r.at(t) - vec3(0, 0, -1));
		return 0.5 * color(surface_normal + vec3(1, 1, 1));
	}

	return sky_color(r);
}

int main() {
	//image
	auto aspect_ratio = 16.0 / 9.0;
	int image_width = 800;
	int image_height = int(image_width / aspect_ratio);
	image_height = (image_height < 1) ? 1 : image_height;

	//camera
	auto focal_length = 1.0;
	auto viewport_height = 2.0;
	auto viewport_width = viewport_height * (double(image_width) / image_height);
	auto camera_center = point3(0, 0, 0);

	auto viewport_u = vec3(viewport_width, 0, 0);
	auto viewport_v = vec3(0, -viewport_height, 0);
	auto pixel_delta_u = viewport_u / image_width;
	auto pixel_delta_v = viewport_v / image_height;

	auto viewport_top_left = camera_center - vec3(0, 0, focal_length) - (viewport_u / 2) - (viewport_v / 2);
	auto pixel0_loc = viewport_top_left + 0.5 * (pixel_delta_u + pixel_delta_v);

	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
	 
	for (int i = 0; i < image_height; i++) {
		std::clog << "\rScanlines remaining: " << (image_height - i) << ' ' << std::flush;
		for (int j = 0; j < image_width; j++) {
			auto pixel_center = pixel0_loc + (j * pixel_delta_u) + (i * pixel_delta_v);
			auto direction = pixel_center - camera_center;
			ray r(camera_center, direction);

			auto pixel_color = ray_color(r);
			write_color(std::cout, pixel_color);
		}
	}

	std::clog << "\rDone\n";
	
	return 0;
}